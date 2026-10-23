/*
 * A190972  a(n) = 7*a(n-1) - 3*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Iterative computation using the recurrence. */
    seq_t prev = 0;   /* a(n-2) */
    seq_t cur  = 1;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = 7 * cur - 3 * prev;  /* a(i) = 7*a(i-1) - 3*a(i-2) */
        prev = cur;
        cur  = next;
    }
    return cur;
}
