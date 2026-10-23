/*
 * A037467  Sum{d(i)*8^i: i=0,1,...,m}, where Sum{d(i)*5^i: i=0,1,...,m} is
 *          the base 5 representation of n.
 * https://oeis.org/A037467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpretation of n's base‑5 digits as a base‑8 number
 *            i.e. a(n) = Σ d_i(n) * 8^i, where n = Σ d_i(n) * 5^i
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t n_local = n;           /* work on copy of input */
    seq_t result = 0;            /* accumulator for the output */
    seq_t multiplier = 1;        /* current power of 8 (8^i) */

    while (n_local > 0) {
        idx_t digit = n_local % 5;    /* base‑5 digit */
        n_local /= 5;

        result += ((seq_t)digit) * multiplier;
        multiplier *= (seq_t)8;
    }
    return result;
}
