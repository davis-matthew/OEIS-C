/*
 * A057139  Odd number of digits palindrome based on sequential digits.
 * https://oeis.org/A057139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of digits (i % 10) for i = 1 … n‑1,
 *            followed by (n % 10), followed by the reverse of the first part.
 *            This yields the odd‑digit sequential palindrome A057139
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t v = 0;

    /* Build the left part (digits 1 … n‑1, modulo 10). */
    for (idx_t i = 1; i < n; ++i) {
        seq_t d = (seq_t)(i % 10);
        v = v * 10 + d;
    }

    /* Middle digit. */
    seq_t mid = (seq_t)(n % 10);
    v = v * 10 + mid;

    /* Build the right part (reverse of the left part). */
    for (idx_t i = n - 1; i > 0; --i) {
        seq_t d = (seq_t)(i % 10);
        v = v * 10 + d;
    }

    return v;
}
