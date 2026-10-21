/*
 * A060577  Number of homeomorphically irreducible general graphs on 2 labeled
 *          nodes and with n edges.
 * https://oeis.org/A060577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=0,1), 4 (n=2), and n*(n+3)/2 - 3 for n > 2
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    /* for n > 2 */
    seq_t sn   = (seq_t)n;
    seq_t prod = sn * ((seq_t)(n + 3));   /* n*(n+3) is always even */
    seq_t half = prod / 2;                /* exact division */
    return half - (seq_t)3;
}
