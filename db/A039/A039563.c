/*
 * A039563  Numbers whose base-5 representation has the same number of 0's,
 *          1's and 2's.
 * https://oeis.org/A039563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 4190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039563: Numbers whose base‑5 representation has the same
   number of 0's, 1's, and 2's.  Computed by brute‑force search. */

#include <stdbool.h>

/* Test whether a number satisfies the condition. */
static inline bool is_good(seq_t x)
{
    if (x == 0)          /* base-5 "0" has 1 zero, 0 ones, 0 twos */
        return false;

    unsigned cnt[3] = {0, 0, 0};
    seq_t t = x;
    while (t > 0) {
        unsigned d = (unsigned)(t % 5);  /* digit in base 5 */
        if (d < 3)
            ++cnt[d];                   /* count only 0, 1, 2 */
        t /= 5;
    }
    return (cnt[0] == cnt[1] && cnt[1] == cnt[2]);
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    idx_t found = 0;   /* how many good numbers we have seen */
    seq_t num   = 1;    /* numbers to test, start from 1 */

    for (;; ++num) {
        if (is_good(num)) {
            ++found;
            if (found == n)
                return num;
        }
    }
}
