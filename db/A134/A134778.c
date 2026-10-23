/*
 * A134778  Last digit of n alphabetically.
 * https://oeis.org/A134778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the digit d of n whose English name is alphabetically
   largest among the digits of n (by lexicographic order on the words
   “zero”…“nine”). */
#include <string.h>

seq_t a(idx_t n)
{
    static const char *word[10] = {
        "zero","one","two","three","four",
        "five","six","seven","eight","nine"
    };

    idx_t tmp = n;
    int best = 0;
    int first = 1;

    do {
        int d = tmp % 10;
        if (first || strcmp(word[d], word[best]) > 0) {
            best = d;
            first = 0;
        }
        tmp /= 10;
    } while (tmp > 0);

    return (seq_t)best;
}
