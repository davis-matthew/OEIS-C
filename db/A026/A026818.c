/*
 * A026818  Largest digit of n concatenated with smallest digit of n is prime.
 * https://oeis.org/A026818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth positive integer k such that the concatenation
 *            of the largest digit of k and the smallest digit of k is a prime
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_concat_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_concat_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of qualifying integers found */
    seq_t num = (seq_t)1;     /* candidate integer to test */

    while (1) {
        seq_t tmp = num;
        int largest = 0;
        int smallest = 9;

        /* Extract each decimal digit of num. */
        while (tmp > (seq_t)0) {
            int d = (int)(tmp % (seq_t)10);   /* digit */
            if (d > largest) largest = d;
            if (d < smallest) smallest = d;
            tmp = tmp / (seq_t)10;            /* shift right one decimal digit */
        }

        int concat = largest * 10 + smallest;
        if (is_concat_prime(concat)) {
            ++count;
            if (count == n) {
                return num;
            }
        }

        num = num + (seq_t)1;   /* advance to next candidate */
    }
}
