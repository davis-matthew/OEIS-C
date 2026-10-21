/*
 * A074612  a(n) = 4^n + 6^n.
 * https://oeis.org/A074612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 6^n * Recurrence form: * a(0) = 2 * a(1) = 10 *
 *            a(n) = 10*a(n-1) - 24*a(n-2) (n >= 2) * * The recurrence uses
 *            only integer arithmetic and works for all builds * because the
 *            intermediate values are exact multiples of the current * term.
 *            The loop runs O(n) times; for n ≤ 10⁴ it is well...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)10;

    seq_t a_nm2 = (seq_t)2;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)10;  /* a(n-1) */
    seq_t a_n;

    /* Iterate up to the requested index using the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm1*10 - a_nm2*24;  /* a(n) = 10*a(n-1) - 24*a(n-2) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
