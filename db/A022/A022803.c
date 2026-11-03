/*
 * A022803  Numbers that reach ...,7,8,4,2,1 under the mapping: if n is even
 *          divide by 2 else add 1.
 * https://oeis.org/A022803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 926 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A022803
 *  Formula: a(n) is the nth smallest positive integer that eventually
 *  reaches 7 when repeatedly applying the mapping f(m) =
 *  m/2 if m is even, otherwise m+1.  The number 7 itself is not
 *  counted. */

#include <stdbool.h>

static inline bool reaches7(seq_t x)
{
    /*  Returns true iff the forward orbit of x under f eventually hits 7.
     *  The orbit stops when it reaches 7 (success) or the 1‑2 cycle
     *  (failure).  All other integers eventually fall into that cycle. */
    for (;;) {
        if (x == (seq_t)7) return true;
        if (x == (seq_t)1 || x == (seq_t)2) return false;

        /*  using modulo to test parity; this works for all seq_t variants. */
        if ((x % (seq_t)2) == (seq_t)0)
            x = x / (seq_t)2;      /* even: divide by 2 */
        else
            x = x + (seq_t)1;      /* odd : add 1 */
    }
}

/*  seq_t  and  idx_t  are supplied by the build system. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* unused, but safe guard */

    idx_t count = 0;

    for (idx_t i = 1; ; ++i) {
        if (i == (idx_t)7) continue;   /* skip 7 itself */

        if (reaches7((seq_t)i)) {
            ++count;
            if (count == n) return (seq_t)i;
        }
    }
}
