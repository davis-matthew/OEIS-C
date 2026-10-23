/*
 * A047477  Numbers that are congruent to {0, 5, 7} mod 8.
 * https://oeis.org/A047477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/3) + r, where r = {0,5,7}[(n-1)%3]
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This generates all non‑negative integers congruent to 0, 5 or 7 modulo 8. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. Behaviour for n == 0 is undefined
       but we return 0 to avoid underflow in the computations below. */
    if (n == 0)
        return 0;

    /* Compute the block index and the position inside the 3‑term cycle. */
    idx_t block = (n - 1) / 3;
    idx_t pos   = (n - 1) % 3;

    /* Determine the remainder in {0,5,7} according to the position. */
    seq_t r = 0;
    switch (pos) {
        case 0: r = 0; break;
        case 1: r = 5; break;
        case 2: r = 7; break;
    }

    /* Final value: 8 * block + r. All arithmetic is performed in seq_t
       to keep the result portable across all supported integer sizes. */
    return (seq_t)block * 8 + r;
}
