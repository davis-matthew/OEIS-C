/*
 * A043434  Numbers having two 3's in base 8.
 * https://oeis.org/A043434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_three_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having two 3’s in base 8.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_three_base8.h"

/* Retrieve the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* the offset is 1; for n < 1 the value is undefined */

    /* Static state: stored terms, count, and current candidate. */
    static seq_t terms[20000];   /* enough for n up to ~10 000 */
    static size_t count = 0;
    static seq_t next_candidate = 0;

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        /* Find the next number with exactly two 3’s in base 8. */
        do {
            ++next_candidate;
        } while (count_three_base8(next_candidate) != 2);

        /* Store the found term. */
        terms[count] = next_candidate;
        ++count;
    }

    return terms[n - 1];
}
