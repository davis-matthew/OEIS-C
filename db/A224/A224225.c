/*
 * A224225  a(0)=1; thereafter a(n) =
 *          2*s(n,1)-3*s(n,2)+4*s(n,4)+9*s(n,6)-36*s(n,12), where s(n,k) =
 *          sigma(n/k) if k divides n, otherwise 0.
 * https://oeis.org/A224225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*s(n,1) - 3*s(n,2) + 4*s(n,4) + 9*s(n,6) - 36*s(n,12), *
 *            where s(n,k) = sigma(n/k) if k | n, otherwise 0
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute sigma(m) = sum of divisors of m, where m is a 64‑bit integer.
 * The implementation uses trial division and thus works for m up to
 * about 10⁷ in a fraction of a millisecond; sufficient for the
 * required range n ≤ 10 000. */
static seq_t sigma_u64(idx_t m)
{
    if (m == 0) return 0;
    seq_t result = 1;
    idx_t n = m;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            seq_t sum = 1;
            seq_t pk   = 1;
            for (idx_t i = 0; i < e; ++i) {
                pk *= (seq_t)p;
                sum += pk;
            }
            result *= sum;
        }
    }
    if (n > 1) {            /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)n);
    }
    return result;
}

/* Requested sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */

    seq_t val = 0;

    /* k = 1, coefficient = 2 */
    {
        seq_t s = sigma_u64(n);
        val += (seq_t)2 * s;
    }

    /* k = 2, coefficient = -3 */
    if ((n & 1ULL) == 0) {
        seq_t s = sigma_u64(n / 2);
        val -= (seq_t)3 * s;
    }

    /* k = 4, coefficient = 4 */
    if ((n & 3ULL) == 0) {
        seq_t s = sigma_u64(n / 4);
        val += (seq_t)4 * s;
    }

    /* k = 6, coefficient = 9 */
    if ((n % 6ULL) == 0) {
        seq_t s = sigma_u64(n / 6);
        val += (seq_t)9 * s;
    }

    /* k = 12, coefficient = -36 */
    if ((n % 12ULL) == 0) {
        seq_t s = sigma_u64(n / 12);
        val -= (seq_t)36 * s;
    }

    return val;
}
