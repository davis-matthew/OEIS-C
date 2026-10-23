/*
 * A018370  Divisors of 279.
 * https://oeis.org/A018370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 279 in ascending order
   a(1) = 1
   a(2) = 3
   a(3) = 9
   a(4) = 31
   a(5) = 93
   a(6) = 279
   For n > 6 the sequence is undefined – the function returns 0. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazily build the list of divisors of 279 once. */
    static seq_t divisors[279]; /* 279 is the number itself – worst-case space */
    static size_t dcnt = 0;     /* number of divisors found */

    if (dcnt == 0) {
        /* Enumerate all i from 1 to 279; those that divide 279 are divisors. */
        for (idx_t i = 1; i <= (idx_t)279; ++i) {
            if ((seq_t)279 % (seq_t)i == (seq_t)0) {
                divisors[dcnt++] = (seq_t)i;        /* already in ascending order */
            }
        }
    }

    if (n == 0 || n > dcnt) {
        return (seq_t)0;
    }

    /* n is 1‑based; convert to 0‑based index. */
    return divisors[(size_t)(n - 1)];
}
