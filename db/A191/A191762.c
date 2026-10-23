/*
 * A191762  Digital roots of the nonzero even squares.
 * https://oeis.org/A191762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root(4*n^2) = ((4*n*n) % 9 == 0 ? 9 : (4*n*n) %
 *            9)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute modulo 9 only, avoiding large intermediate values */
    idx_t rem = n % 9;                  /* n mod 9  (value 0..8) */
    seq_t m = (seq_t)rem;               /* cast to seq_t for further ops */
    seq_t sq_mod = (m * m) % 9;         /* (n^2) mod 9 */
    seq_t val_mod = (4 * sq_mod) % 9;   /* (4*n^2) mod 9 */
    return val_mod == 0 ? (seq_t)9 : val_mod;  /* convert 0 to 9 for digital root */
}
