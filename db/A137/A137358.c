/*
 * A137358  a(n) = Sum_{k <= n/2} binomial(n-2k, 3k+2).
 * https://oeis.org/A137358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k <= n/2} binomial(n-2k, 3k+2) Recurrence: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=3, a(4)=6, a(n) = 3*a(n-1) - 3*a(n-2) +
 *            a(n-3) + a(n-5) for n >= 5
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 42, u64 n <= 84, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of OEIS sequence A137358. */
seq_t a(idx_t n)
{
    /* Handle the first five values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)6;

    /* Ring of the last five computed values.  Initially they
       correspond to a(0) .. a(4). */
    seq_t a0 = (seq_t)0;  /* a(n-5) */
    seq_t a1 = (seq_t)0;  /* a(n-4) */
    seq_t a2 = (seq_t)1;  /* a(n-3) */
    seq_t a3 = (seq_t)3;  /* a(n-2) */
    seq_t a4 = (seq_t)6;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) + a(i-5) */
        seq_t val = (seq_t)3 * a4 - (seq_t)3 * a3 + a2 + a0;

        /* Shift the ring for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = val;
    }

    return a4;   /* a(n) */
}
