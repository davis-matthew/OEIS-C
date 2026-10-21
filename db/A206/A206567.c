/*
 * A206567  S(m,n) = (number of nonzero terms common to the base 3 expansions
 *          of m and n), a symmetric matrix read by antidiagonals.
 * https://oeis.org/A206567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 9678, u16 n <= 9678, u32 n <= 9678, u64 n <= 9678,
 *            u128 n <= 9678, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computing A206567: a(n) is the number of common non‑zero digits
   (the same value, 1 or 2) in the base‑3 expansions of the two indices
   that correspond to the nth antidiagonal entry of the symmetric
   matrix S(m,n).  The first two indices m,n start at 1.  */

/* Index n (≥1) is converted to the pair (i,j):
       d  = smallest integer with (d‑1)(d‑2)/2 < n ≤ (d‑1)d/2   (diag. number)
       t  = d‑1   (length of the diagonal)
       pos = n – t(t‑1)/2   (1‑based position along the diagonal)
       i   = pos
       j   = t+1 – pos
   Then a(n) = S(i,j).  */

/* The arithmetic for large values uses unsigned __int128 to avoid
   overflows.  For integer square roots of 128‑bit numbers a binary
   search routine is used.  */

#include <stdint.h>
#include <stdbool.h>

/* integer square root of an unsigned 128‑bit value */
static uint64_t isqrt_uint128(unsigned __int128 v)
{
    uint64_t x = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t bit = 1ULL << i;
        uint64_t tmp = x | bit;
        if ((unsigned __int128)tmp * tmp <= v)
            x = tmp;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined in OEIS, return 0 for safety */
        return (seq_t)0;

    /* find t = d-1: length of the diagonal that contains n */
    unsigned __int128 v = (unsigned __int128)8 * n + 1;
    uint64_t sq = isqrt_uint128(v);               /* floor(sqrt(8n+1)) */
    uint64_t t = (sq - 1) / 2;                    /* floor((sqrt-1)/2) */

    unsigned __int128 tri = (unsigned __int128)t * (t + 1) / 2;
    if (tri < n)                       /* if we didn't reach n yet */
        t++;                           /* move to the next diagonal */

    uint64_t d = t + 1;                /* actual diagonal number */

    unsigned __int128 before = (unsigned __int128)t * (t - 1) / 2;
    idx_t pos = n - (idx_t)before;     /* position within the diagonal */
    idx_t i = pos;
    idx_t j = d - pos;

    /* compute the number of common non‑zero base‑3 digits */
    seq_t count = 0;
    while (i > 0 || j > 0) {
        unsigned int di = i % 3;
        unsigned int dj = j % 3;
        if (di && dj && di == dj)
            ++count;
        i /= 3;
        j /= 3;
    }
    return count;
}
