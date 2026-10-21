/*
 * A069256  Size of the Sylow 2-subgroup of the group GL_2(Z_n): maximal power
 *          of 2 that divides A000252(n).
 * https://oeis.org/A069256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative function * a(1)=1, * a(2^e)=2^(4e-3) (e≥1), * for
 *            an odd prime power p^e : a(p^e)=2^v2((p−1)(p^2−1))
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses trial‑division factorisation; fast enough for n≤10 000. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;        /* a(1)=1, a(0) unused */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* factor out powers of 2 */
    int e2 = 0;
    while ((m & 1ULL) == 0) {
        ++e2;
        m >>= 1;
    }
    if (e2 > 0) {
        /* a(2^e2)=2^(4e2-3) */
        idx_t power = 4 * e2 - 3;
        seq_t val = (seq_t)1;
        for (idx_t i = 0; i < power; ++i)
            val <<= 1;          /* multiply by 2 */
        result *= val;
    }

    /* factor remaining odd part */
    while (m > 1) {
        idx_t p = 0;
        /* trial division up to sqrt(m) */
        for (idx_t d = 3; d * d <= m; d += 2) {
            if (m % d == 0) {
                p = d;
                break;
            }
        }
        if (p == 0)        /* m is prime */
            p = m;

        /* remove whole prime power (exponent irrelevant for odd p) */
        while (m % p == 0)
            m /= p;

        /* compute v2((p-1)(p^2-1)) for odd prime p */
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        uint64_t v2val = (uint64_t)(p - 1) * (pp - 1);

        int v2 = 0;
        while ((v2val & 1ULL) == 0) {
            ++v2;
            v2val >>= 1;
        }
        seq_t mult = ((seq_t)1) << v2;
        result *= mult;
    }

    return result;
}
