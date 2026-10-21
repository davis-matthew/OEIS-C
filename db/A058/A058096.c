/*
 * A058096  McKay-Thompson series of class 9d for Monster.
 * https://oeis.org/A058096
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 99, i32 n <= 342, i64 n <= 1224,
 *            i128 n <= 4545, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A058096
   Formula:  a(n) = coefficient of q^n in   (A - 3/A)
   where   A = q^{-1} * P^2  and   P = ∏_{k≥1} (1 - q^k)^{e(k)}
   with e(k) =  0                          , k % 3 ≠ 0
                -1 if k % 3 = 0 and k % 9 ≠ 0
                 1 if k % 9 = 0 and k % 27 ≠ 0
                 0 if k % 27 = 0
   The coefficient a(n) for n ≥ 0 is:
        a(n) = Q[n+1] – 3 * InvQ[n−1]   (InvQ[m] is the coefficient of q^m in 1/Q)
   For n = –1   a(–1) = 1   and for n < –1 the value is arbitrary (here 0).  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static size_t cache_cap = 0;
static seq_t *p_arr = NULL;     /* coefficients of P,   p[0] = 1  */
static seq_t *q_arr = NULL;     /* Q = P * P             */
static seq_t *inv_arr = NULL;   /* inverse of Q          */

/* Extend the cached1 arrays to at least ‘new_cap’ entries.
   The arrays are rebuilt from scratch each time an extension is needed. */
static void ensure_cap(size_t new_cap)
{
    if (new_cap <= cache_cap) return;

    /* Allocate new arrays */
    seq_t *p_new   = seq_alloc_new_array(new_cap);
    seq_t *q_new   = seq_alloc_new_array(new_cap);
    seq_t *inv_new = seq_alloc_new_array(new_cap);

    /* Free old arrays (if any) */
    if (p_arr) seq_free_array(p_arr);
    if (q_arr) seq_free_array(q_arr);
    if (inv_arr) seq_free_array(inv_arr);

    p_arr   = p_new;
    q_arr   = q_new;
    inv_arr = inv_new;

    /* Initialise arrays */
    for (size_t i = 0; i < new_cap; ++i)
        p_arr[i] = 0, q_arr[i] = 0, inv_arr[i] = 0;
    p_arr[0] = 1;        /* P starts with 1 */

    /* Build P :  P = ∏ (1 - q^k)^{e(k)}   (e(k) as defined above) */
    for (size_t k = 1; k < new_cap; ++k) {
        int e;
        if (k % 3 != 0)                      e = 0;
        else if (k % 9 != 0)                 e = -1;
        else if (k % 27 == 0)                e = 0;
        else                                 e = 1;

        if (e == 1) {                         /* multiply by (1 - q^k)  */
            for (size_t m = new_cap - 1; m >= k; --m)
                p_arr[m] -= p_arr[m - k];
        }
        else if (e == -1) {                   /* multiply by 1/(1-q^k)  */
            for (size_t m = k; m < new_cap; ++m)
                p_arr[m] += p_arr[m - k];
        }
    }

    /* Compute Q = P * P  (convolution) */
    for (size_t i = 0; i < new_cap; ++i) {
        for (size_t j = 0; i + j < new_cap; ++j)
            q_arr[i + j] += p_arr[i] * p_arr[j];
    }

    /* Compute the inverse series of Q:  Q * InvQ = 1   (Q[0] = 1) */
    inv_arr[0] = 1;                           /* 1/1  */
    for (size_t m = 1; m < new_cap; ++m) {
        seq_t sum = 0;
        for (size_t i = 1; i <= m; ++i) {
            sum += q_arr[i] * inv_arr[m - i];
        }
        inv_arr[m] = -sum;                    /* since Q[0] == 1 */
    }

    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == -1) return 1;          /* a(-1) = 1   */
    if (n < -1)  return 0;          /* arbitrary but safe */

    /* we need indices up to n+1 (Q) and n-1 (InvQ)  */
    size_t needed = (size_t)(n + 2);   /* +1 for Q[n+1], +1 to be safe   */
    ensure_cap(needed);

    seq_t result = q_arr[n + 1];          /* coefficient Q[n+1]  */
    if (n >= 1)                          /* subtract 3 * InvQ[n-1] */
        result -= (seq_t)3 * inv_arr[n - 1];

    return result;
}
