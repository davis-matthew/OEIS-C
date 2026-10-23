/*
 * A126826  Ramanujan numbers (A000594) read mod 9.
 * https://oeis.org/A126826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*sigma(n) mod 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n^2 mod 9 */
    idx_t nmod = n % 9;
    seq_t n_sq_mod = (seq_t)((nmod * nmod) % 9);

    /* sigma(n) mod 9 by explicit divisor enumeration */
    seq_t sigma_mod = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            if (d == d2) {
                sigma_mod += (seq_t)(d % 9);
            } else {
                sigma_mod += (seq_t)(d % 9);
                sigma_mod += (seq_t)(d2 % 9);
            }
            sigma_mod %= 9;  /* keep within 0..8 */
        }
    }

    /* a(n) = n^2 * sigma(n) mod 9 */
    seq_t result = (n_sq_mod * sigma_mod) % 9;
    return result;
}
