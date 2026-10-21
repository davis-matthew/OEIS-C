/*
 * A093393  [n/9] + [n/4] + [(n+1)/9] + [(n+1)/4] + [(n+2)/9].
 * https://oeis.org/A093393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n/9] + [n/4] + [(n+1)/9] + [(n+1)/4] + [(n+2)/9]
 * Range:     u8 n <= 262, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;           /* convert index to sequence type */
    return (nn / 9) + (nn / 4) + ((nn + 1) / 9) + ((nn + 1) / 4) + ((nn + 2) / 9);
}
