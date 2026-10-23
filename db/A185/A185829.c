/*
 * A185829  Half the number of nX3 binary arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Sequence A185829: half the number of n×3 binary arrays where each
 * element has exactly one or two equal horizontal/vertical neighbors.
 *
 * Empirical recurrence with 14 previous terms (index offset 1):
 *   a(n) = 2a(n-1)+6a(n-2)-a(n-3)-3a(n-4)+4a(n-5)-4a(n-6)-32a(n-7)
 *          -9a(n-8)-4a(n-9)-19a(n-10)+17a(n-11)+3a(n-12)+a(n-13)-a(n-14)
 *
 * Initial terms (offset 1):
 *   a(1)=1, a(2)=10, a(3)=30, a(4)=118, a(5)=393, a(6)=1430,
 *   a(7)=5004, a(8)=17877, a(9)=63181, a(10)=224324, a(11)=794939,
 *   a(12)=2819380, a(13)=9995857, a(14)=35445025
 *
 * Sequence values fit in seq_t for the requested range.
 */

static const seq_t init_vals[14] = {
    (seq_t)1,      (seq_t)10,     (seq_t)30,     (seq_t)118,
    (seq_t)393,    (seq_t)1430,   (seq_t)5004,   (seq_t)17877,
    (seq_t)63181,  (seq_t)224324, (seq_t)794939, (seq_t)2819380,
    (seq_t)9995857,(seq_t)35445025
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    if (n <= 14) return init_vals[n-1];

    /* For 64‑bit seq_t we use 128‑bit intermediate arithmetic to avoid
     * overflow.  For larger seq_t types we use the arithmetic of the
     * type itself.  The recurrence uses only small integer coefficients. */
    if (sizeof(seq_t) <= 8) {
        const int coeff[14] = { 2, 6, -1, -3, 4, -4, -32, -9, -4, -19,
                               17, 3, 1, -1 };
        __int128_t val[14];
        for (int i = 0; i < 14; ++i) val[i] = (__int128_t)init_vals[i];

        for (idx_t k = 15; k <= n; ++k) {
            __int128_t v = 0;
            for (int i = 0; i < 14; ++i)
                v += (__int128_t)coeff[i] * val[13 - i];
            /* shift window left */
            for (int i = 0; i < 13; ++i) val[i] = val[i + 1];
            val[13] = v;
        }
        return (seq_t)val[13];
    } else {
        seq_t val[14];
        for (int i = 0; i < 14; ++i) val[i] = init_vals[i];

        for (idx_t k = 15; k <= n; ++k) {
            /* accumulate positive contributions first, then subtract negatives */
            seq_t v = val[13] * (seq_t)2
                    + val[12] * (seq_t)6
                    + val[9]  * (seq_t)4
                    + val[3]  * (seq_t)17
                    + val[2]  * (seq_t)3
                    + val[1];
            v -= val[11];
            v -= val[10] * (seq_t)3;
            v -= val[8]  * (seq_t)4;
            v -= val[7]  * (seq_t)32;
            v -= val[6]  * (seq_t)9;
            v -= val[5]  * (seq_t)4;
            v -= val[4]  * (seq_t)19;
            v -= val[0];

            /* shift window left */
            for (int i = 0; i < 13; ++i) val[i] = val[i + 1];
            val[13] = v;
        }
        return val[13];
    }
}
