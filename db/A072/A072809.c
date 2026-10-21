/*
 * A072809  Abntu sequence (Bantu alphabetized): list the numbers which have
 *          no 2's in their decimal expansion, but sort the digits of each
 *          term into alphabetical order.
 * https://oeis.org/A072809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word, base
 * Formula:   a(n) = sorted digits of the n-th integer (in increasing order)
 *            that contains no digit 2, sorted according to the order
 *            8,5,4,9,1,7,6,3,0. The digits of the original number are
 *            rearranged following that custom alphabetic order
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is defined by:
    1. Enumerate all non‑negative integers that do not contain the digit 2.
    2. For each such integer, sort its decimal digits in the order
       8 < 5 < 4 < 9 < 1 < 7 < 6 < 3 < 0.
    3. Output the resulting numbers in the order they were enumerated. */

#include <stdbool.h>

/* Order in which the digits are arranged when sorted.
   The list is in ascending order with respect to the "alphabet" */
static const unsigned char ORDER_DIGITS[9] = {8,5,4,9,1,7,6,3,0};

seq_t a(idx_t n) {
    idx_t count = 0;      /* how many valid numbers have been seen */
    idx_t num   = 0;      /* current candidate integer */

    while (true) {
        /* Count the frequency of each digit in 'num'.
           If the digit '2' appears, the number is discarded. */
        unsigned short freq[10] = {0};
        bool bad = false;
        idx_t t = num;

        if (t == 0) {
            /* Handle the special case of 0 explicitly */
            freq[0] = 1;
        } else {
            while (t > 0) {
                unsigned d = t % 10;
                if (d == 2) {
                    bad = true;
                    break;
                }
                freq[d]++;
                t /= 10;
            }
        }

        if (!bad) {
            count++;
            if (count == n) {
                /* Construct the sorted result by emitting the digits
                   in the order specified by ORDER_DIGITS. */
                seq_t result = 0;
                for (int i = 0; i < 9; ++i) {
                    unsigned d = ORDER_DIGITS[i];
                    unsigned short f = freq[d];
                    while (f--) {
                        result = result * 10 + d;
                    }
                }
                return result;
            }
        }
        num++;
    }
}
