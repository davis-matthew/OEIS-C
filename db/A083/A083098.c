/*
 * A083098  a(n) = 2*a(n-1) + 6*a(n-2).
 * https://oeis.org/A083098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 6*a(n-2),
 * a(0) = 1, a(1) = 1
 */
seq_t a(idx_t n)
{
    /* Handle the two base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* a(n-2) and a(n-1) are stored in a0 and a1 respectively */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t an;             /* a(n) */

    /* Iterate from n = 2 up to the requested n */
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * 2 + a0 * 6;
        a0 = a1;
        a1 = an;
    }

    return a1;  /* a(n) */
}
