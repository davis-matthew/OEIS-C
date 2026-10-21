/*
 * A237516  Pyramidal centered square numbers.
 * https://oeis.org/A237516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { Σ_{i=1}^{2*(n-1)*n+1} i } = M = 2*(n-1)*n + 1; a(n) =
 *            M*(M+1)/2 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* convert n to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t sn_minus1 = sn - 1;          /* s(n-1) */
    seq_t M = 2 * sn_minus1 * sn + 1;  /* M = 2*(n-1)*n + 1 */
    seq_t result = M * (M + 1) / 2;    /* sum 1..M */
    return result;
}
