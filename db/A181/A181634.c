/*
 * A181634  Irregular triangle T(n,k) where row n contains all pairs [a,b] of
 *          the compositions n=a+b into nonnegative pairs where a == b (mod
 *          3).
 * https://oeis.org/A181634
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   a_max_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the terms of OEIS A181634.
   The sequence is obtained by flattening the rows of the triangle
   T(n,·) where row n contains all ordered pairs [a,b] such that
   a+b = n and a ≡ b (mod 3).  Within each row the pairs appear
   in descending order of a, and each pair contributes its two
   entries consecutively: a then b.
   The first term of the sequence has index 2.  This routine
   returns the value a(n) for any n ≥ 2. */

#include <stdint.h>   /* for uint64_t in case idx_t == uint64_t */
#include "../utils/a_max_val.h"

static inline idx_t count_pairs(idx_t r)
{
    /* number of (a,b) pairs in row r (ordered pairs) */
    idx_t r_mod = r % 3;                 /* 0,1,2 */
    idx_t r0    = (r_mod + r_mod) % 3;   /* 2*r mod 3, but safe from overflow */
    return ((r - r0) / 3) + 1;
}

seq_t a(idx_t n)
{
    if (n < 2)                /* offset is 2; values below are undefined */
        return (seq_t)0;

    idx_t idx = n - 2;        /* 0‑based position in the flattened sequence */
    idx_t r   = 2;            /* start from row 2 */
    idx_t cum = 0;            /* cumulative length of previous rows */

    while (1)
    {
        idx_t len = count_pairs(r) * 2;
        if (cum + len > idx)
            break;
        cum += len;
        ++r;
    }

    idx_t pos = idx - cum;    /* 0‑based position within row r */
    idx_t a_max = a_max_val(r);

    if ((pos & 1) == 0)       /* even position → a component */
    {
        idx_t j = pos >> 1;   /* j = position of the pair within the row */
        idx_t val = a_max - 3 * j;
        return (seq_t)val;
    }
    else                      /* odd position → b component */
    {
        idx_t j = pos >> 1;
        idx_t val = r - (a_max - 3 * j);
        return (seq_t)val;
    }
}
