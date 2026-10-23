/*
 * A193068  Generating primitive Pythagorean triangles by using (n, n+1) gives
 *          perimeters for each n. This sequence lists the sum of these
 *          perimeters for each n triangles.
 * https://oeis.org/A193068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^2 + 15*n + 17)/3
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;
#if defined(__SIZEOF_INT128__)
    __uint128_t nn = n;
    __uint128_t val = nn * (( (__uint128_t)4 * nn * nn )
                             + ( (__uint128_t)15 * nn )
                             + 17);
    val /= 3;
    return (seq_t)val;
#else
    seq_t n_seq = (seq_t)n;
    if (n % 3 == 0) {
        seq_t n3 = n_seq / 3;
        seq_t n_sq = n_seq * n_seq;
        seq_t t = 4 * n_sq + 15 * n_seq + 17;
        return n3 * t;
    } else {
        seq_t n_sq = n_seq * n_seq;
        seq_t t = 4 * n_sq + 15 * n_seq + 17;
        seq_t t3 = t / 3;
        return n_seq * t3;
    }
#endif
}
