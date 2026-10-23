/*
 * A109841  a(n) is the minimal j >= n such that the sum of consecutive
 *          integers from n to j is a palindrome.
 * https://oeis.org/A109841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the minimal j ≥ n such that the sum of consecutive
 *            integers from n to j is a palindrome
 * Range:     u8 n <= 11, u16 n <= 683, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_palindrome_seq.h"

/* Main function computing the sequence. */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)n;   /* current sum of n..j */
    idx_t j = n;            /* current end of the interval */

    for (;;) {
        if (is_palindrome_seq(sum)) {
            return (seq_t)j;
        }
        ++j;
        sum += (seq_t)j;
    }
}
