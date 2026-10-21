/*
 * A081852  Consider recurrence b(0) = n/3, b(n) = b(0)*ceiling(b(n-1));
 *          sequence gives first integer reached (or -1 if no integer is ever
 *          reached).
 * https://oeis.org/A081852
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 28, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: b(0) = n/3,  b(k) = b(0) * ceil(b(k-1))
 * a(n)  = first integer reached (or –1 if never). */

/* Assumes seq_t supports + - * / % and comparisons. */
seq_t a(idx_t n)
{
    /* undefined for n < 3; return 0. */
    if (n < 3) return 0;

    /* If n is a multiple of 3, the starting value is already an integer. */
    if (n % 3 == 0)
        return n / 3;

    /* Work with the numerator m so that
     *   b(k) = m / 3
     * and the recurrence on m is      m_next = n * ceil(m/3). */
    seq_t m = n;
    while (m % 3 != 0) {
        seq_t ceil = (m + 2) / 3;   /* ceil(m/3) */
        m = n * ceil;                /* next numerator */
    }
    return m / 3;                   /* first integer reached */
}
