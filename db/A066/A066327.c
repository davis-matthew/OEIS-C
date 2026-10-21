/*
 * A066327  Binary string which equals n when 1's, 2's, 4's and 8's bits have
 *          weights -1, 1, 3, 6 respectively, while the other bits have their
 *          usual weights. -1 if no such string exists.
 * https://oeis.org/A066327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) is the decimal value of a binary string whose weighted sum
 *            of bits equals n. Bits 0‑3 have weights -1,1,3,6; bits 4 and
 *            above have weights 2^i. If no such string exists, a(n) = -1
 * Range:     i8 n <= 4, i16 n <= 30, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Mapping of n mod 16 to the bits 0‑3 that realize the residue.
   0xFF marks an invalid residue (no solution).               */
static const unsigned char pattern_map[16] = {
    0x00, /* 0  -> 0       */
    0x02, /* 1  -> 10      */
    0x05, /* 2  -> 101     */
    0x04, /* 3  -> 100     */
    0x06, /* 4  -> 110     */
    0x09, /* 5  -> 1001    */
    0x08, /* 6  -> 1000    */
    0x0A, /* 7  -> 1010    */
    0x0D, /* 8  -> 1101    */
    0x0C, /* 9  -> 1100    */
    0x0E, /*10  -> 1110    */
    0xFF, /*11  -> no sol  */
    0xFF, /*12  -> no sol  */
    0xFF, /*13  -> no sol  */
    0xFF, /*14  -> no sol  */
    0x01  /*15  -> 10001   */
};

seq_t a(idx_t n)
{
    /* Separate the multiple of 16 from the remainder. */
    idx_t base = n / 16u;
    unsigned r = (unsigned)(n - base * 16u);

    /* Check if the remainder has a solution. */
    unsigned char patt = pattern_map[r];
    if (patt == (unsigned char)0xFF)
        return (seq_t)-1;

    /* For residue 15 we need one extra 16 and a special pattern. */
    if (r == 15) {
        base += 1u;
        patt = 0x01;           /* bit0 only */
    }

    /* Find the most significant set bit in the whole string. */
    int msb = -1;
    for (int i = 63; i >= 0; --i) {
        bool set = false;
        if (i >= 4) {
            set = (bool)(((unsigned long long)base >> (i - 4)) & 1ULL);
        } else {
            set = (bool)((patt >> i) & 1U);
        }
        if (set) {
            msb = i;
            break;
        }
    }

    /* If no bits were set, n must be 0 → return decimal 0. */
    if (msb < 0)
        return (seq_t)0;

    /* Convert the bit pattern to a decimal number whose digits are
       the bits of the string. */
    seq_t val = 0;
    for (int i = msb; i >= 0; --i) {
        bool set = false;
        if (i >= 4) {
            set = (bool)(((unsigned long long)base >> (i - 4)) & 1ULL);
        } else {
            set = (bool)((patt >> i) & 1U);
        }
        val = val * 10 + (set ? 1 : 0);
    }
    return val;
}
