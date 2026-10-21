/*
 * A058821  Dimensions of homogeneous subspaces of shuffle algebra over
 *          6-letter alphabet (see A058766 for 2-letter case).
 * https://oeis.org/A058821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, for n>=2 a(n) = 6^n - (1/n) * Σ_{d|n} μ(d) *
 *            6^{n/d} where μ is the Möbius function
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
   - Uses only integer arithmetic.
   - Computes 6^k for all k≤n once in an array.
   - mobius() is computed by trial division.
   - All operations are performed on seq_t and idx_t.
   - No static libraries are used other than <stdlib.h>.  */

#include <stdlib.h>

/* Compute Möbius μ(d) for 1<=d<=n.
   Returns 0 for non‑square‑free, 1 for even number of primes, -1 otherwise. */
static int mobius(idx_t d)
{
    if (d == 1) return 1;
    int cnt = 0;
    idx_t x = d;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            ++cnt;
            x /= p;
            if (x % p == 0) return 0;   /* square factor */
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;

    /* pre‑compute powers of 6: pow6[k] = 6^k, 0<=k<=n */
    seq_t *pow6 = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!pow6) abort();           /* insufficient memory – terminate */
    pow6[0] = 1;
    for (idx_t i = 1; i <= n; ++i) pow6[i] = pow6[i - 1] * (seq_t)6;

    seq_t pos = 0;   /* sum of 6^{n/d} for μ(d)=+1 */
    seq_t neg = 0;   /* sum of 6^{n/d} for μ(d)=-1 */

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i, d2 = n / i;

            int mu1 = mobius(d1);
            if (mu1 == 1) {
                pos += pow6[n / d1];
            } else if (mu1 == -1) {
                neg += pow6[n / d1];
            }

            if (d1 != d2) {
                int mu2 = mobius(d2);
                if (mu2 == 1) {
                    pos += pow6[n / d2];
                } else if (mu2 == -1) {
                    neg += pow6[n / d2];
                }
            }
        }
    }

    seq_t diff = pos;
    diff -= neg;                     /* diff = Σ μ(d) * 6^{n/d}  (always positive) */
    seq_t primitive = diff / (seq_t)n;    /* primitive necklaces count A032164(n) */
    seq_t result = pow6[n] - primitive;   /* a(n) = 6^n - primitive */

    free(pow6);
    return result;
}
