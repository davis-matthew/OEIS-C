/*
 * A062033  Binary expansion of n does not contain 1-bits at even positions.
 *          Integers whose base 4 representation consists of only 0's and 2s.
 * https://oeis.org/A062033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by writing the binary expansion of n, replacing
 *            each 1 by the decimal string “10” and each 0 by “00”; the
 *            resulting decimal string is interpreted as a decimal integer.
 *            Example: n = 5 (binary 101) → “100010” → a(5) = 100010
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n == 0) return 0;               /* a(0) = 0 */

    /* Find most significant set bit of n (0‑based). */
    int msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1) msb++;            /* msb = floor(log2(n)) */

    /* For each bit, append “10” or “00” to the decimal representation. */
    for (int i = msb; i >= 0; --i) {
        res = res * (seq_t)100 + /* shift decimal by two digits */
              ((n & ((idx_t)1 << i)) ? (seq_t)10 : (seq_t)0);
    }
    return res;
}
