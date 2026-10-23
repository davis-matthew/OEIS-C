/*
 * A018919  Define the generalized Pisot sequence T(a(0),a(1)) by: a(n+2) is
 *          the greatest integer such that a(n+2)/a(n+1) < a(n+1)/a(n). This
 *          is T(3,9).
 * https://oeis.org/A018919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) for n ≥ 3 with a(0) = 3, a(1) = 9, a(2)
 *            = 26
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)26;

    seq_t a0 = (seq_t)3;   /* a(n-3) */
    seq_t a1 = (seq_t)9;   /* a(n-2) */
    seq_t a2 = (seq_t)26;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = (seq_t)3 * a2 - a0; /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
