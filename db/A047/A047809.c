/*
 * A047809  a(n) counts different values of i^2+j^2+k^2 <= n^2 or number of
 *          distances from the origin to all integer points inside a sphere of
 *          radius n.
 * https://oeis.org/A047809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = #{0 ≤ m ≤ n² | m is a sum of three integer squares}
 * Range:     u8 n <= 15, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using Legendre's theorem: non‑representable m are of the form 4^a(8b+7) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* N = n² */
    seq_t N = (seq_t)n * (seq_t)n;

    /* total numbers from 0 to N inclusive */
    seq_t total = N + (seq_t)1;

    /* count non‑representable numbers ≤ N */
    seq_t nonrep = (seq_t)0;
    seq_t power  = (seq_t)1;          /* 4^a */

    while (power <= N) {
        if (N / power >= (seq_t)7) {
            seq_t t   = N / power;                    /* t = 8b+7 */
            seq_t cnt = (t - (seq_t)7) / (seq_t)8 + (seq_t)1;
            nonrep   += cnt;
        }
        /* prevent overflow of power */
        if (power > N / (seq_t)4)
            break;
        power *= (seq_t)4;
    }

    return total - nonrep;
}
