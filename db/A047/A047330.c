/*
 * A047330  Numbers that are congruent to {0, 1, 3, 5, 6} mod 7.
 * https://oeis.org/A047330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1)/5)*7 + {0,1,3,5,6}[ (n-1)%5 ]
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0   a(2) = 1   a(3) = 3   a(4) = 5   a(5) = 6   ... */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined offset, return 0 for safety */
    idx_t m = n - 1;
    idx_t q = m / 5;
    idx_t r = m % 5;
    /* base = number of complete 7‑block cycles */
    seq_t base = (seq_t)q * 7;
    static const seq_t residues[5] = {0, 1, 3, 5, 6};
    return base + residues[r];
}
