/*
 * A014101  a(n) = a(n-1) + a(n-4), starting 1,1,1,3.
 * https://oeis.org/A014101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 68, u64 n <= 137,
 *            u128 n <= 275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-4);
   Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=3 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)3;

    /* keep the last four computed terms */
    seq_t v0 = (seq_t)1; /* a(0) */
    seq_t v1 = (seq_t)1; /* a(1) */
    seq_t v2 = (seq_t)1; /* a(2) */
    seq_t v3 = (seq_t)3; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = v3 + v0;   /* a(i) = a(i-1) + a(i-4) */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3; /* a(n) */
}
