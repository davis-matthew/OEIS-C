/*
 * A093509  Multiples of 5 or 6.
 * https://oeis.org/A093509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(n‑1)/10⌋·30 + r[(n‑1) mod 10] where r =
 *            {0,5,6,10,12,15,18,20,24,25} is the pattern of residues that are
 *            multiples of 5 or 6 modulo 30
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Table of residues in increasing order. */
    static const seq_t r[10] = {
        0, 5, 6, 10, 12, 15, 18, 20, 24, 25
    };

    /* n ≥ 1 by the problem statement; no underflow for n‑1. */
    idx_t n_minus_one = n - 1u;
    idx_t block = n_minus_one / 10u;          /* complete groups of 10 terms */
    unsigned int idx = (unsigned int)(n_minus_one % 10u); /* 0..9 */

    /* Compute the value; cast idx to seq_t and use seq_t for multiplication. */
    return r[idx] + (seq_t)30u * (seq_t)block;
}
