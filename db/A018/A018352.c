/*
 * A018352  Divisors of 244.
 * https://oeis.org/A018352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The sequence is the list of positive divisors of 244 in increasing order.
   a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 61, a(5) = 122, a(6) = 244. */

seq_t a(idx_t n)
{
    /* Static cache of the 6 divisors of 244 */
    static seq_t divisors[6];
    static size_t divisor_count = 0;

    if (divisor_count == 0) {
        /* Generate divisors 2^a * 61^b with 0≤a≤2, 0≤b≤1 */
        size_t pos = 0;
        for (size_t a = 0; a <= 2; ++a) {
            seq_t pow2 = ((seq_t)1) << a;          /* 1, 2, 4 */
            for (size_t b = 0; b <= 1; ++b) {
                seq_t val = pow2 * ((b == 0) ? ((seq_t)1) : ((seq_t)61));
                divisors[pos++] = val;
            }
        }
        divisor_count = pos;

        /* Simple insertion sort for the small array */
        for (size_t i = 1; i < divisor_count; ++i) {
            size_t j = i;
            while (j > 0 && divisors[j - 1] > divisors[j]) {
                seq_t tmp = divisors[j];
                divisors[j] = divisors[j - 1];
                divisors[j - 1] = tmp;
                --j;
            }
        }
    }

    if (n >= 1 && n <= divisor_count)
        return divisors[(size_t)(n - 1)];
    return (seq_t)0;   /* out of bounds */
}
