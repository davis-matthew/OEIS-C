/*
 * A041219  Denominators of continued fraction convergents to sqrt(120).
 * https://oeis.org/A041219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 22*a(n-2) - a(n-4); a(0)=1, a(1)=1, a(2)=21, a(3)=22
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for any seq_t (fixed‑width or arbitrary‑precision)
   and for any idx_t (uint64_t).  No dynamic memory allocation is used. */
seq_t a(idx_t n)
{
    /* handle the first four values directly */
    if (n == 0u || n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)21;
    if (n == 3u) return (seq_t)22;

    /* rolling buffer for a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a0 */
    buf[1] = (seq_t)1;   /* a1 */
    buf[2] = (seq_t)21;  /* a2 */
    buf[3] = (seq_t)22;  /* a3 */

    /* compute a(i) iteratively using the recurrence */
    for (idx_t i = 4u; i <= n; ++i) {
        seq_t next = (seq_t)22 * buf[2] - buf[0];  /* a(i-2) and a(i-4) */
        /* shift buffer left: a(i-3) becomes a(i-4), etc. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }

    return buf[3];
}
