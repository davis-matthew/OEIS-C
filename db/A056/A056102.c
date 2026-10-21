/*
 * A056102  Numbers k such that k^4 == 1 (mod 5^5).
 * https://oeis.org/A056102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A056102: numbers k such that k⁴ ≡ 1 (mod 5⁵ = 3125).
   The 4 residue classes modulo 3125 are 1, 1068, 2057, 3124.
   All such numbers are of the form r + 3125·q where q ≥ 0.
   They are listed in increasing order:
       a(n) = 3125 * ⌊(n-1)/4⌋ + [1,1068,2057,3124][(n-1) mod 4]   (n ≥ 1) */
seq_t a(idx_t n)
{
    /* there are exactly 4 solutions modulo 3125, thus a linear formula applies. */
    static const seq_t residues[4] = { (seq_t)1,
                                       (seq_t)1068,
                                       (seq_t)2057,
                                       (seq_t)3124 };
    idx_t m = n - 1;                     /* shift to 0‑indexed */
    seq_t q = (seq_t)(m / 4);             /* integer division, safe for all n ≥ 1 */
    idx_t r = m % 4;                      /* 0…3 */
    return (seq_t)3125 * q + residues[r];
}
