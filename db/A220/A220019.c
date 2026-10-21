/*
 * A220019  Number of cyclotomic cosets of 7 mod 10^n.
 * https://oeis.org/A220019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 5*(16*n^2 - 60*n + 37) for n > 3; a(1) = 4, a(2) = 27,
 *            a(3) = 93
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base_vals[4] = {0, 4, 27, 93}; /* base_vals[0] unused */
    if (n <= 3)
        return base_vals[n];

    seq_t tn = (seq_t)n;
    seq_t term = 16 * tn * tn - 60 * tn + 37;
    return 5 * term;
}
