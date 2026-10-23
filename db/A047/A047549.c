/*
 * A047549  Numbers that are congruent to {0, 1, 2, 3, 4, 7} mod 8.
 * https://oeis.org/A047549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + [0,1,2,3,4,7][(n-1)%6]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _a047549_offsets[6] = {0, 1, 2, 3, 4, 7};

seq_t a(idx_t n) {
    /* Return the nth non‑negative integer congruent to 0,1,2,3,4 or 7 modulo 8. */
    if (n == 0)
        return 0;  /* undefined for n=0, return 0 as a safety guard */

    idx_t q = (n - 1) / 6;     /* number of complete 6‑term blocks before n */
    idx_t r = (n - 1) % 6;     /* position within the current block */

    return (seq_t)8 * (seq_t)q + _a047549_offsets[r];
}
