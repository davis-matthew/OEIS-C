/*
 * A231431  Evil squares.
 * https://oeis.org/A231431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A231431 – Evil squares
 *
 *  a(n) is the n‑th square whose binary representation contains an even
 *  number of 1’s (i.e. the square is an "evil number").
 *  The sequence starts with a(1)=0.
 */

#include <stdbool.h>

static inline bool even_popcount(seq_t x)
{
    bool parity = false;
    while (x) {
        /* parity ^= (number of 1's in the least significant bit) */
        parity ^= (x & 1) != 0;
        x >>= 1;
    }
    return !parity;           /* true when popcount is even   */
}

seq_t a(idx_t n)
{
    idx_t m = 0;              /* current integer whose square is considered */
    idx_t cnt = 0;            /* how many evil squares we've found so far     */

    for (;;) {
        seq_t sq = (seq_t)m * m;
        if (even_popcount(sq)) {
            ++cnt;
            if (cnt == n)
                return sq;
        }
        ++m;
    }
}
