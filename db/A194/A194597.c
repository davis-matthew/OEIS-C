/*
 * A194597  Digital roots of the nonzero hexagonal numbers.
 * https://oeis.org/A194597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root(n*(2*n-1)) = 1 + ((n*(2*n-1) - 1) % 9)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses modular arithmetic modulo 9 to avoid overflow. */
seq_t a(idx_t n)
{
    /*  n mod 9  */
    seq_t n_mod9 = ((seq_t)n) % 9;

    /*  (2*n) mod 9  */
    seq_t two_n_mod9 = (n_mod9 * 2) % 9;

    /*  (2*n - 1) mod 9  */
    seq_t two_n_minus1_mod9 = (two_n_mod9 + 8) % 9;   /* subtract 1 */

    /*  n*(2*n - 1) mod 9  */
    seq_t hm9 = (n_mod9 * two_n_minus1_mod9) % 9;

    /*  digital root: 9 if remainder is 0, otherwise the remainder itself  */
    seq_t dr = (hm9 == 0) ? 9 : hm9;
    return dr;
}
