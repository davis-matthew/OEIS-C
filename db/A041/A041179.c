/*
 * A041179  Denominators of continued fraction convergents to sqrt(99).
 * https://oeis.org/A041179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041179 – Denominators of the continued‑fraction convergents to sqrt(99)
   Formula:   a(0)=1, a(1)=1, a(2)=19, a(3)=20
              a(n)=20*a(n-2) – a(n-4)   for n ≥ 4
   Recurrence keys: recursion only involves the two previous even and even‑2 indices,
   so we can compute a(n) in O(n) time and O(1) memory using a circular buffer of
   the last four values. */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)19;
    if (n == 3) return (seq_t)20;

    /* Circular buffer for the last four values: a[i%4] holds a(i). */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a0 */
    buf[1] = (seq_t)1;   /* a1 */
    buf[2] = (seq_t)19;  /* a2 */
    buf[3] = (seq_t)20;  /* a3 */

    /* Iterate from 4 up to n, updating the buffer. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = (seq_t)20 * buf[(i - 2) % 4] - buf[(i - 4) % 4];
        buf[i % 4] = val;
    }

    /* The result is stored in buf[n%4]. */
    return buf[n % 4];
}
