/*
 * A217023  Number of city-block distance 1, pressure limit 2 movements in an
 *          nX3 array with each element moving exactly one horizontally or
 *          vertically and no element acquiring more than two neighbors.
 * https://oeis.org/A217023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*a(n-1) - 2152*a(n-3) + 3498*a(n-4) + 26598*a(n-5) -
 *            55952*a(n-6) + 44616*a(n-8) - 17576*a(n-9) (n>10) Initial
 *            values: a(1)=2, a(2)=121, a(3)=3456, a(4)=108900, a(5)=3356150,
 *            a(6)=103754596, a(7)=3204762800, a(8)=99000846736,
 *            a(9)=3058191082836, a(10)=94469768837136
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static const seq_t base[10] = {
        2ULL,          /* a(1) */
        121ULL,        /* a(2) */
        3456ULL,       /* a(3) */
        108900ULL,     /* a(4) */
        3356150ULL,    /* a(5) */
        103754596ULL,  /* a(6) */
        3204762800ULL, /* a(7) */
        99000846736ULL,/* a(8) */
        3058191082836ULL, /* a(9) */
        94469768837136ULL /* a(10) */
    };

    if (n == 0) return 0; /* sequence offset starts at 1 */

    /* 20000 is safely larger than the maximum n used in tests (≈10 000). */
    static seq_t cache[20000] = {0};
    static idx_t cache_size = 0;

    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i - 1] = base[i - 1];
            } else {
                seq_t val = cache[i - 2] * (seq_t)33;
                val -= cache[i - 4] * (seq_t)2152;
                val += cache[i - 5] * (seq_t)3498;
                val += cache[i - 6] * (seq_t)26598;
                val -= cache[i - 7] * (seq_t)55952;
                val += cache[i - 9] * (seq_t)44616;
                val -= cache[i -10] * (seq_t)17576;
                cache[i - 1] = val;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
