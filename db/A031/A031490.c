/*
 * A031490  Numbers whose base-8 representation has one fewer 0 than 7's.
 * https://oeis.org/A031490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate integers and collect those whose base‑8 representation
   contains one fewer 0 than 7’s (i.e., #7 == #0 + 1). */
#include <stdlib.h>

static inline int has_property(seq_t x)
{
    unsigned zeros = 0;
    unsigned sevens = 0;
    seq_t tmp = x;
    while (tmp != (seq_t)0) {
        unsigned digit = (unsigned)(tmp % (seq_t)8);
        if (digit == 0) ++zeros;
        else if (digit == 7) ++sevens;
        tmp /= (seq_t)8;
    }
    return sevens == zeros + 1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;      /* offset starts at 1 */

    seq_t candidate = (seq_t)1;
    idx_t found = (idx_t)0;

    while (found < n) {
        if (has_property(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
    return (seq_t)0;   /* unreachable */
}
