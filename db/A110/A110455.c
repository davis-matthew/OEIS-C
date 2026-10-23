/*
 * A110455  Integers with mutual residues -6.
 * https://oeis.org/A110455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7, a(2)=13, a(3)=85. For n≥4: a(n) = a(n−1)^2 + 6·a(n−1) –
 *            6
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No headers are required beyond the standard integer types that
   are defined in the build environment.  seq_t and idx_t are supplied
   externally. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 (offset 1) */

    /* A static array that can hold more than 10 000 terms */
    #define MAX_CAP 20000
    static seq_t seq[MAX_CAP];
    static size_t cur = 0;          /* number of terms computed so far   */

    /* initialise the first three values */
    if (cur == 0) {
        seq[0] = (seq_t)7;
        seq[1] = (seq_t)13;
        seq[2] = (seq_t)85;
        cur = 3;
    }

    /* extend the cache up to the requested index */
    while (cur < (size_t)n) {
        seq_t  prev = seq[cur - 1];
        seq_t  six  = (seq_t)6;
        seq[cur] = prev * prev + six * prev - six;
        ++cur;
    }

    return seq[n - 1];
}
