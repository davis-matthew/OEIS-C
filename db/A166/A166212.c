/*
 * A166212  Number of 4 X 4 X 4 triangular nonnegative integer arrays,
 *          symmetric under 120 degree rotation, with all sums of an element
 *          and its neighbors <= n.
 * https://oeis.org/A166212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=14, a(4)=25, a(5)=39, a(6)=66,
 *            a(7)=99, a(8)=138, a(9)=201, a(10)=274, a(11)=357, a(12)=479,
 *            a(13)=616, a(14)=768; For n>13: a(n)=2*a(n‑1) - a(n‑2) +
 *            2*a(n‑3) - 4*a(n‑4) + 2*a(n‑5) - 2*a(n‑9) + 4*a(n‑10) -
 *            2*a(n‑11) + a(n‑12) - 2*a(n‑13) + a(n‑14) Recurrence order
 *            14,...
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* First 15 terms (n = 0 .. 14). */
    static const seq_t base[15] = {
        1ULL, 3ULL, 6ULL, 14ULL, 25ULL, 39ULL, 66ULL,
        99ULL, 138ULL, 201ULL, 274ULL, 357ULL, 479ULL,
        616ULL, 768ULL
    };

    /* If n is small, return from the base array. */
    if (n <= 14)
        return base[(size_t)n];

    /* Circular buffer of the last 15 values; initialized with base. */
    seq_t buf[15];
    for (size_t i = 0; i < 15; ++i)
        buf[i] = base[i];

    /* Compute terms from 15 up to n. */
    for (idx_t i = 15; i <= n; ++i) {
        /* Indices modulo 15. */
        size_t i1 = (size_t)((i - 1) % 15);
        size_t i2 = (size_t)((i - 2) % 15);
        size_t i3 = (size_t)((i - 3) % 15);
        size_t i4 = (size_t)((i - 4) % 15);
        size_t i5 = (size_t)((i - 5) % 15);
        size_t i9 = (size_t)((i - 9) % 15);
        size_t i10 = (size_t)((i - 10) % 15);
        size_t i11 = (size_t)((i - 11) % 15);
        size_t i12 = (size_t)((i - 12) % 15);
        size_t i13 = (size_t)((i - 13) % 15);
        size_t i14 = (size_t)((i - 14) % 15);

        /* Use a signed intermediate to avoid unsigned underflow. */
        __int128_t val = 0;
        val +=  2 * (__int128_t)buf[i1];
        val -=  1 * (__int128_t)buf[i2];
        val +=  2 * (__int128_t)buf[i3];
        val -=  4 * (__int128_t)buf[i4];
        val +=  2 * (__int128_t)buf[i5];
        val -=  2 * (__int128_t)buf[i9];
        val +=  4 * (__int128_t)buf[i10];
        val -=  2 * (__int128_t)buf[i11];
        val +=  1 * (__int128_t)buf[i12];
        val -=  2 * (__int128_t)buf[i13];
        val +=  1 * (__int128_t)buf[i14];

        buf[(size_t)(i % 15)] = (seq_t)val;
    }

    return buf[(size_t)(n % 15)];
}
