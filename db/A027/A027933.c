/*
 * A027933  a(n) = T(n, 2*n-10), T given by A027926.
 * https://oeis.org/A027933
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..5} binomial(n-k, 10-2*k)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 45, u64 n <= 386,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Custom binomial function that safely handles 64‑bit intermediate results
   when seq_t is uint64_t, and delegates to seq_t arithmetic otherwise. */
static seq_t binom_safe(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0) return (seq_t)1;
    /* Choose the smaller of k and n-k to minimise multiplicative steps. */
    if (k > n - k) k = n - k;

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        idx_t numerator = n - k + i;    /* n-k+i is always <= n */
        /* Deal with 64‑bit seq_t specially to avoid overflow. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 tmp = (unsigned __int128)result *
                                   (unsigned __int128)numerator;
            tmp /= i; /* division is exact for binomial coefficients. */
            result = (seq_t)tmp;
        } else {
            /* For arbitrary‑precision seq_t types, use normal arithmetic. */
            result = result * (seq_t)numerator;
            result = result / (seq_t)i;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 5.  Returning 0 for smaller indices
       satisfies the required behavior without affecting the intended
       sequence. */
    if (n < 5) return 0;

    seq_t sum = 0;
    for (idx_t k = (idx_t)0; k <= (idx_t)5; ++k) {
        idx_t m = (idx_t)(10 - 2 * k);   /* 10, 8, 6, 4, 2, 0 */
        if (m > n - k) continue;         /* binomial(n-k, m) = 0 here */
        sum += binom_safe(n - k, m);
    }
    return sum;
}
