/*
 * A038139  Order of n (mod 9).
 * https://oeis.org/A038139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = multiplicative order of n modulo 9; return 0 if gcd(n,9)
 *            ≠ 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Mapping of residues modulo 9 to the order of the residue
       when it is coprime to 9; 0 is used for residues not coprime. */
    static const seq_t order[9] = {0, 1, 6, 0, 3, 6, 0, 3, 2};
    return order[n % 9];
}
