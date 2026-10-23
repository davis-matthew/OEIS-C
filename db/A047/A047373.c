/*
 * A047373  Numbers that are congruent to {0, 1, 2, 3, 5} mod 7.
 * https://oeis.org/A047373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/5) + {0,1,2,3,5}[(n-1)%5]
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n ≥ 1) */
seq_t a(idx_t n)
{
    static const seq_t offs[5] = {0, 1, 2, 3, 5};
    idx_t q = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    seq_t base = (seq_t)q * (seq_t)7;
    return base + offs[r];
}
