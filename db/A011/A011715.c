/*
 * A011715  A binary m-sequence: expansion of reciprocal of x^8+x^6+x^5+x^4+1.
 * https://oeis.org/A011715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) XOR a(n-6) XOR a(n-5) XOR a(n-4); initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t tbl[255];
    static int ready = 0;
    if (!ready) {
        /* initialise first 255 terms via the recurrence */
        tbl[0] = tbl[1] = tbl[2] = tbl[3] = tbl[4] = tbl[5] = tbl[6] = 0;
        tbl[7] = 1;
        for (size_t i = 8; i < 255; ++i) {
            tbl[i] = tbl[i-8] ^ tbl[i-6] ^ tbl[i-5] ^ tbl[i-4];
        }
        ready = 1;
    }
    size_t k = (size_t)(n % 255);
    return tbl[k];
}
