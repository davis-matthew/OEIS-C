/*
 * A035734  Coordination sequence for 39-dimensional cubic lattice.
 * https://oeis.org/A035734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0}^{min(39,n)} binom(39,i) * binom(38 + n - i, n -
 *            i)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* multiplicative binomial coefficient.
   Uses 128‑bit intermediate to avoid overflow for 64‑bit seq_t. */
static seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;          /* symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* compute res = res * (N-K+i) / i with 128‑bit safety        */
        #if defined(__SIZEOF_UNSIGNED_INT128__)
            __uint128_t temp = (__uint128_t)res * (N - K + i);
            temp /= i;
            res = (seq_t)temp;
        #else
            /* fallback (may overflow for very large N) */
            res = res * (N - K + i) / i;
        #endif
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Pre‑compute binomial(39,i) once.  We use a static local array. */
    static seq_t C39[40];
    static int initialized = 0;
    if (!initialized) {
        for (idx_t i = 0; i <= 39; ++i) {
            C39[i] = binom(39, i);
        }
        initialized = 1;
    }

    seq_t sum = 0;
    idx_t limit = n < 39 ? n : 39;
    for (idx_t i = 0; i <= limit; ++i) {
        seq_t term1 = C39[i];
        seq_t term2 = binom(38 + n - i, n - i);

        /* multiply with 128‑bit safety */
        #if defined(__SIZEOF_UNSIGNED_INT128__)
            __uint128_t prod = (__uint128_t)term1 * term2;
            sum += (seq_t)prod;
        #else
            sum += term1 * term2;
        #endif
    }
    return sum;
}
