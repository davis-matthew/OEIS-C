/*
 * A083816  10^n-th palindrome.
 * https://oeis.org/A083816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(1) = 11, a(2) = 919, a(3) = 90109, a(4) = 9001009;
 *            for n ≥ 5, a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)919;
    if (n == 3) return (seq_t)90109;
    if (n == 4) return (seq_t)9001009;

    /* Iterate using the recurrence, keeping the last three values. */
    seq_t a_nm3 = (seq_t)919;      /* a(n-3) when n=5 */
    seq_t a_nm2 = (seq_t)90109;    /* a(n-2) when n=5 */
    seq_t a_nm1 = (seq_t)9001009;  /* a(n-1) when n=5 */
    seq_t a_n    = 0;

    for (idx_t i = 5; i <= n; ++i) {
        a_n = (seq_t)(111 * a_nm1) - (seq_t)(1110 * a_nm2) + (seq_t)(1000 * a_nm3);
        /* Shift the window for next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
