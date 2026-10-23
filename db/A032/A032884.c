/*
 * A032884  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(0)
 *          < d(1) > d(2) < ...
 * https://oeis.org/A032884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A032884: Numbers whose base‑5 representation has digits
 * alternating up‑down: d0 < d1 > d2 < d3 > … (least significant digit first)
 * Offset: 1 (a(1)=1).  The function tests each integer until it finds the n‑th
 * value satisfying the pattern; no memoisation is used.
 */
#include <stdbool.h>

/* Return true if x has alternating up‑down digits in base 5. */
static bool is_valid(seq_t x)
{
    /* Zero is not considered a positive integer in the sequence. */
    if (x == 0) return false;

    /* Extract the least significant digit first. */
    seq_t temp  = x;
    seq_t prev  = temp % 5;
    temp       = temp / 5;
    int pos    = 0;   /* position of prev (0 for least significant digit) */

    while (temp > 0)
    {
        seq_t curr = temp % 5;
        temp     = temp / 5;

        /* For even positions we need prev < curr,
           for odd positions we need prev > curr. */
        if ((pos & 1) == 0)   /* even position */
        {
            if (!(prev < curr)) return false;
        }
        else                  /* odd position */
        {
            if (!(prev > curr)) return false;
        }

        prev = curr;
        ++pos;
    }

    return true;
}

/* Return the n-th term of A032884 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* harmless for undefined case */

    idx_t   count     = 0;            /* how many valid numbers found */
    seq_t   candidate = 1;            /* current integer to test */

    while (count < n)
    {
        if (is_valid(candidate))
            ++count;
        ++candidate;                  /* move to next integer */
    }

    return candidate - 1;             /* last valid number found */
}
