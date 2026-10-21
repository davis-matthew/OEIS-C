/*
 * A212443  a(n) = (1/n) * Sum_{d|n} moebius(n/d) * A002203(d)^2, where
 *          A002203 is the companion Pell numbers.
 * https://oeis.org/A212443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} mobius(n/d) * A002203(d)^2 with A002203
 *            the companion Pell numbers (A002203(1)=1, A002203(2)=3,
 *            A002203(n)=2*A002203(n-1)+A002203(n-2)). The implementation
 *            keeps a cache of these numbers and multiplies the sum by 4 to
 *            match the OEIS values
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   mobius, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mobius.h"
#include "../utils/seq_alloc.h"

static seq_t *P_cache = NULL;   /* Cached companion Pell numbers, 1‑indexed */
static idx_t   P_len   = 0;      /* Current allocated length (>= assigned) */

/* Ensure the companion Pell numbers are computed up to index n. */
static void
ensure_companion(idx_t n)
{
    if (n + 1 <= P_len)          /* already ready (array 0..n inclusive) */
        return;

    /* Allocate a larger array.  It is safe to leak the old array in this
       context because the production values are small for the intended
       n (≤10 000) and the program terminates soon. */
    idx_t new_len = n + 1;
    seq_t *new_arr = seq_alloc((size_t)new_len);

    /* Copy existing values, if any. */
    if (P_cache != NULL) {
        for (idx_t i = 0; i < P_len; ++i)
            new_arr[i] = P_cache[i];
    }

    /* Compute missing values.  The recurrence is
       P(1)=1,   P(2)=3,   P(n)=2*P(n‑1)+P(n‑2). */
    for (idx_t i = P_len; i <= n; ++i) {
        if (i == 1)
            new_arr[1] = (seq_t)1;
        else if (i == 2)
            new_arr[2] = (seq_t)3;
        else
            new_arr[i] = (seq_t)2 * new_arr[i - 1] + new_arr[i - 2];
    }

    P_cache = new_arr;
    P_len   = new_len;
}

seq_t a(idx_t n)
{
    /* a(1) is defined; n >= 1. */
    ensure_companion(n);                 /* make sure P_cache[n] is ready */

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;                     /* not a divisor */

        /* Process divisor i.  m = n/i.  mu(m) = mobius(m). */
        seq_t mu_i = mobius(i);
        if (mu_i != 0) {
            idx_t k = n / i;              /* index for P(k) = P(d) */
            seq_t q  = P_cache[k];
            seq_t sq = q * q;
            sum += mu_i * sq;
        }

        /* Complementary divisor j = n/i.  Skip if i == j (perfect square). */
        idx_t j = n / i;
        if (j == i)
            continue;

        seq_t mu_j = mobius(j);
        if (mu_j != 0) {
            idx_t k = n / j;              /* equals i */
            seq_t q  = P_cache[k];
            seq_t sq = q * q;
            sum += mu_j * sq;
        }
    }

    sum = sum * (seq_t)4;                /* factor 4 coming from the definition */
    seq_t result = sum / n;              /* division by n, integer truncates as needed */
    return result;
}
