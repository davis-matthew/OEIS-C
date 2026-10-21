/*
 * A230193  Numbers divisible by 3 or 11.
 * https://oeis.org/A230193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+13) = a(n) + 33; first 13 terms: 3,6,9,11,12,15,18,21,22,24,27,30,33 */
seq_t a(idx_t n)
{
    /* For n == 0 the result is irrelevant; return 0 for safety. */
    if (n == 0)
        return 0;

    static const seq_t first[13] = {
        3, 6, 9, 11, 12, 15, 18, 21, 22, 24, 27, 30, 33
    };

    idx_t m = n - 1;          /* convert to zero‑based index */
    idx_t pos   = m % 13;     /* position within one period of length 13 */
    idx_t cycles = m / 13;    /* how many complete periods before this term */

    return first[pos] + (seq_t)cycles * (seq_t)33;
}
