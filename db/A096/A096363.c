/*
 * A096363  Length of repeating cycle of the final n digits in the Fibonacci
 *          sequence.
 * https://oeis.org/A096363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1) = 60; a(2) = 300; a(n) = 15 * 10^(n‑1) for n > 2 (Pisano
 *            period of Fibonacci modulo 10^(n-1))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)60;
    if (n == 2) return (seq_t)300;
    seq_t res = (seq_t)15;
    /* multiply by 10 a total of (n-1) times */
    idx_t i = 0;
    while (i < n - 1) {
        res = res * ((seq_t)10);
        ++i;
    }
    return res;
}
