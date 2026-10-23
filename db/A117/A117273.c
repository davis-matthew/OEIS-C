/*
 * A117273  Numbers m such that the product of the digits of m is equal to the
 *          number of primes less than m.
 * https://oeis.org/A117273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117273: Numbers m such that the product of the digits of m is equal
   to the number of primes less than m.  The sequence is finite; the
   following array contains all previously known terms (offset 1). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence from the OEIS entry:
        16, 53, 63, 364, 437, 545, 573, 829, 963,
        5449, 6475, 23797, 67458, 2475998   */
    static const seq_t seq[] = {
        16, 53, 63, 364, 437, 545, 573, 829, 963,
        5449, 6475, 23797, 67458, 2475998
    };
    const idx_t len = sizeof(seq) / sizeof(seq[0]);

    if (n >= 1 && n <= len)
        return seq[n - 1];
    /* For n beyond the known range, the sequence is undefined;
       return 0 as a sentinel value. */
    return 0;
}
