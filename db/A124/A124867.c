/*
 * A124867  Numbers that are the sum of 3 distinct primes.
 * https://oeis.org/A124867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10,12,14,15,16 for n = 1…5; * a(n) = n + 12 for all n ≥ 6
 *            (the conjecture that every integer >17 is * the sum of three
 *            distinct primes ensures that all numbers from 18 upwards *
 *            belong to the sequence)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t first5[] = {10, 12, 14, 15, 16};

    if (n == 0)          /* undefined, return 0 for safety */
        return 0;

    if (n <= 5)          /* the first five terms are special */
        return first5[n - 1];

    /* From n ≥ 6 all integers ≥ 18 occur exactly once in the sequence. */
    return (seq_t)(n + 12);
}
