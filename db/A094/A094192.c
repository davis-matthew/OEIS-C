/*
 * A094192  Values x of the generator pairs (x, y), x>y of primitive
 *          Pythagorean triples, sorted.
 * https://oeis.org/A094192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th value of m such that there exists an integer k
 *            (0 < k < m) with gcd(m, k) = 1 and m and k have opposite parity.
 *            Each m occurs as many times as the number of such k's
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute φ(m) for a small integer m. */
static seq_t phi_small(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t tmp = m;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1)
        result -= result / tmp;
    return result;
}

/* Return the n-th term of sequence A094192. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* out of range; harmless for offset 1 work */

    seq_t cum = 0;
    for (idx_t m = 2; ; ++m) {
        seq_t phi = phi_small(m);
        seq_t count = (m & 1) ? (phi / 2) : phi;   /* odd m: φ/2, even m: φ */
        cum += count;
        if (cum >= n)
            return (seq_t)m;
    }
}
