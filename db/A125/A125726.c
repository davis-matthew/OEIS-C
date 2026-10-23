/*
 * A125726  Call n Egyptian if we can partition n = x_1+x_2+...+x_k into
 *          positive integers x_i such that Sum_{i=1..k} 1/x_i = 1; sequence
 *          gives Egyptian numbers.
 * https://oeis.org/A125726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 13 for n ≥ 12; the first 11 terms are 1, 4, 9, 10,
 *            11, 16, 17, 18, 20, 22, 24
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n‑th Egyptian number.  */
seq_t a(idx_t n)
{
    static const seq_t first[11] = {1, 4, 9, 10, 11, 16, 17, 18, 20, 22, 24};
    if (n <= 11)
        return first[n - 1];
    return n + 13;
}
