/*
 * A047548  Numbers that are congruent to {1, 2, 3, 4, 7} mod 8.
 * https://oeis.org/A047548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers that are congruent to {1, 2, 3, 4, 7} modulo 8. The n‑th
 *            term is obtained by: b = (n-1)/5 // number of complete blocks of
 *            5 terms r = (n-1)%5 // position inside the block (0‑based) a(n)
 *            = 8*b + d[r] // d = {1,2,3,4,7} For r <= 3, d[r] = r+1,
 *            otherwise d[4] = 7
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; behaviour for n < 1 is arbitrary */
    static const seq_t d[5] = {1, 2, 3, 4, 7};

    idx_t base = (n - 1) / 5;
    idx_t r    = (n - 1) % 5;
    /* cast to seq_t before multiplication to avoid overflow and
       to support arbitrary precision builds */
    seq_t result = ((seq_t)8) * (seq_t)base + d[r];
    return result;
}
