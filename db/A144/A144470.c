/*
 * A144470  Triangle t(n,m) read by rows: t(n,m) = binomial(n,m)*3^m if m <=
 *          n/2, else t(n,m) = t(n,n-m).
 * https://oeis.org/A144470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A144470:  t(n,m) = binomial(n,m)*3^m for 0≤m≤n/2;
    t(n,m) = t(n,n‑m) for m>n/2.
    The sequence lists the triangle read by rows:
    a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=6, a(5)=1, …       */
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Find the row r such that indices < r*(r+1)/2 belong to earlier rows,
       and the current row r contains indices from r*(r+1)/2 to (r+1)(r+2)/2 - 1. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Position of n within its row */
    idx_t offset = n - r * (r + 1) / 2;      /* 0 ≤ offset ≤ r */

    /* Determine the exponent of 3 using the symmetry rule. */
    idx_t exp = (offset <= r / 2) ? offset : r - offset;

    /* Compute binomial(r, offset) multiplicatively. */
    seq_t binom = 1;
    for (idx_t i = 1; i <= offset; ++i) {
        binom = binom * (seq_t)(r - offset + i) / (i);
    }

    /* Compute 3^exp using mul_small. */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow3 = mul_small(pow3, 3);

    /* Result */
    return binom * pow3;
}
