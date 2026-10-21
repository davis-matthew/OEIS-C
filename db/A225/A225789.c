/*
 * A225789  Least triangular number of the form p*triangular(n) where p is a
 *          prime number, or 0 if no such triangular number exists.
 * https://oeis.org/A225789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least triangular number of the form p *
 *            triangular(n) where p is a prime number, or 0 if no such
 *            triangular number exists
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Helper: test a divisor m of N and update the best triangular found. */
static inline void consider_m(idx_t m, idx_t N, bool *found, seq_t *best)
{
    idx_t t = N / m;                     /* complementary divisor */

    /* Case 1: N = m * (p*m + 1)  →  p = (t - 1) / m */
    if ((t - 1) % m == 0) {
        idx_t p = (t - 1) / m;
        if (p > 1 && is_prime_small((seq_t)p)) {
            idx_t k = p * m;            /* k satisfies k(k+1) = p*N */
            seq_t T = (seq_t)k * (k + 1) / 2;
            if (!*found || T < *best) {
                *best = T;
                *found = true;
            }
        }
    }

    /* Case 2: N = m * (p*m - 1)  →  p = (t + 1) / m */
    if ((t + 1) % m == 0) {
        idx_t p = (t + 1) / m;
        if (p > 1 && is_prime_small((seq_t)p)) {
            idx_t k = p * m - 1;
            if (k == 0) return;          /* skip non‑positive k */
            seq_t T = (seq_t)k * (k + 1) / 2;
            if (!*found || T < *best) {
                *best = T;
                *found = true;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 by definition */

    idx_t N = n * (n + 1);                /* N = n(n+1) */
    bool found = false;
    seq_t best = 0;

    for (idx_t d = 1; (unsigned __int128)d * d <= N; ++d) {
        if (N % d != 0) continue;

        consider_m(d, N, &found, &best);
        idx_t m2 = N / d;                 /* complementary divisor */
        if (m2 != d) consider_m(m2, N, &found, &best);
    }

    return found ? best : 0;
}
