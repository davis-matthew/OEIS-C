/*
 * A057846  Sort the digits of n into alphabetical order (the "Obsessive
 *          Filer's Sequence").
 * https://oeis.org/A057846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, word, look, easy
 * Formula:   a(n) = sort_digits_alphabetical(n) The digits are sorted
 *            according to the alphabetical order of their English names,
 *            which in numeric order is: 8,5,4,9,1,7,6,3,2,0. a(0)=0
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count decimal digits of n. */
    int cnt[10] = {0};
    if (n == 0) {
        cnt[0] = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            ++cnt[d];
            tmp /= 10;
        }
    }

    /* Order of digits based on alphabetic names:
       8(four),5(eight),4(four),9(nine),1(one),7(seven),
       6(six),3(three),2(two),0(zero) */
    const int order[10] = {8,5,4,9,1,7,6,3,2,0};

    seq_t result = 0;
    for (int i = 0; i < 10; ++i) {
        int d = order[i];
        int c = cnt[d];
        while (c--) {
            result = result * 10 + d;
        }
    }
    return result;
}
