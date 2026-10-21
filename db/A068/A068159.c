/*
 * A068159  a(n) = floor[ n/R(n) ], where R(n) (A004086) = Digit reversal of
 *          n.
 * https://oeis.org/A068159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor( n / R(n) ) where R(n) is the decimal digit
 *            reversal of n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Calculate the decimal digit reversal of n using 64‑bit arithmetic. */
    idx_t tmp = n;
    idx_t rev = 0;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* Perform the division in seq_t arithmetic. */
    seq_t n_s  = (seq_t)n;
    seq_t rev_s = (seq_t)rev;   /* rev is non‑zero because n ≥ 1 */

    return n_s / rev_s;         /* integer division gives floor */
}
