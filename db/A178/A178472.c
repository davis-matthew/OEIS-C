/*
 * A178472  Number of compositions (ordered partitions) of n where the gcd of
 *          the part sizes is not 1.
 * https://oeis.org/A178472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} 2^(d-1) * (-mu(n/d))
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- utility: Möbius function for small integers ---------- */
static int mu_of_idx(idx_t m) {
    if (m == 1) return 1;              /* μ(1) = 1 */
    int cnt = 0;                       /* number of distinct prime factors */
    for (idx_t p = 2; p <= m / p; ++p) {  /* loop up to sqrt(m) */
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 1) return 0;   /* square factor ⇒ μ = 0 */
            }
            ++cnt;
        }
    }
    if (m > 1) ++cnt;                  /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;    /* μ = (-1)^k  */
}

/* ---------- main sequence function ---------- */
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;               /* a(1) = 0 */

    seq_t sum = 0;

    for (idx_t i = 1; i <= n / i; ++i) {      /* iterate through divisors */
        if (n % i != 0)
            continue;

        idx_t d1 = i;
        idx_t d2 = n / i;

        /* process divisor d1 (if less than n) */
        if (d1 < n) {
            int mu = mu_of_idx(n / d1);
            if (mu != 0) {
                seq_t term = pow2(d1 - 1);   /* 2^(d1-1) */
                if (mu == -1)
                    sum += term;           /* -mu = +1 */
                else
                    sum -= term;           /* -mu = -1 */
            }
        }

        /* process paired divisor d2, ensure distinct and < n */
        if (d2 != d1 && d2 < n) {
            int mu = mu_of_idx(n / d2);
            if (mu != 0) {
                seq_t term = pow2(d2 - 1);
                if (mu == -1)
                    sum += term;
                else
                    sum -= term;
            }
        }
    }

    return sum;
}
