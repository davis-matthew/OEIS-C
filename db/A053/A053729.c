/*
 * A053729  Self-convolution of 1,4,27,256,3125,46656,... (cf. A000312).
 * https://oeis.org/A053729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k^k * (n+1-k)^(n+1-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Auxiliary function: exponentiation by squaring for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != (idx_t)0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* Cache of k^k values, indexes start at 1 → f_arr[0] = 1^1 */
static seq_t *f_arr = NULL;
static idx_t  f_size = 0;  /* number of filled elements (original n) */
static idx_t  f_cap  = 0;

/* Cache of a(n), indexes start at 1 → a_arr[0] = a(1) */
static seq_t *a_arr = NULL;
static idx_t  a_size = 0;
static idx_t  a_cap  = 0;

/* Ensure that f_arr contains values up to n (inclusive). */
static void ensure_f(idx_t n)
{
    if (n <= f_size) return;
    idx_t new_cap = f_cap ? f_cap : 4;
    while (new_cap < n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure – terminate */
    f_arr = tmp;
    f_cap = new_cap;

    for (idx_t i = f_size; i < n; ++i) {
        /* i corresponds to k = i+1 */
        f_arr[i] = pow_seq_t((seq_t)(i + 1), (idx_t)(i + 1));
    }
    f_size = n;
}

/* Ensure that a_arr contains values up to n (inclusive). */
static void ensure_a(idx_t n)
{
    if (n <= a_size) return;
    idx_t new_cap = a_cap ? a_cap : 4;
    while (new_cap < n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_arr = tmp;
    a_cap = new_cap;

    for (idx_t i = a_size; i < n; ++i) {
        /* i corresponds to n = i+1 */
        idx_t len = i + 1;
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < len; ++j) {
            /* f_arr[j] = (j+1)^(j+1)          */
            /* f_arr[len-1-j] = (n-j)^(n-j)    */
            sum += f_arr[j] * f_arr[len - 1 - j];
        }
        a_arr[i] = sum;
    }
    a_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, graceful */
    ensure_f(n);                          /* need k^k for k=1..n */
    ensure_a(n);                          /* compute a(1)..a(n) if not yet */
    return a_arr[n - 1];
}
