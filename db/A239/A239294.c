/*
 * A239294  a(n) = (15^n - (-3)^n)/18.
 * https://oeis.org/A239294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15^n - (-3)^n)/18
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalent recurrence: a(0) = 0, a(1) = 1, a(n) = 12*a(n-1) + 45*a(n-2)  (n >= 2) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)0;   /* a(n-2)   */
    seq_t a1 = (seq_t)1;   /* a(n-1)   */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = a1 * (seq_t)12 + a0 * (seq_t)45;
        a0 = a1;
        a1 = next;
    }

    return a1;
}
