/*
 * A100136  a(n) = Sum_{k=0..floor(n/6)} C(n-3k,3k) * 3^k.
 * https://oeis.org/A100136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 77, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 3*a(n-6)
 * Initial values:  a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=1  */
seq_t a(idx_t n)
{
    /* All terms for n < 6 are 1. */
    if (n < 6)
        return (seq_t)1;

    /* Circular buffer to hold the last six computed values. */
    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = (seq_t)1;   /* a(0) .. a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        idx_t   idx = i % 6;               /* position for a(i) */
        seq_t a1 = buf[(i - 1) % 6];       /* a(i-1) */
        seq_t a2 = buf[(i - 2) % 6];       /* a(i-2) */
        seq_t a3 = buf[(i - 3) % 6];       /* a(i-3) */
        seq_t a6 = buf[(i - 6) % 6];       /* a(i-6) */

        /* Compute the next value using the recurrence. */
        seq_t next = 3 * a1;               /* 3*a(i-1)   */
        next += a3;                       /* + a(i-3)   */
        next += 3 * a6;                   /* + 3*a(i-6) */
        next -= 3 * a2;                   /* - 3*a(i-2) */

        buf[idx] = next;                  /* store a(i) */
    }

    return buf[n % 6];
}
