/*
 * A025985  Expansion of 1/((1-2*x)*(1-4*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A025985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1)-278*a(n-2)+976*a(n-3)-1056*a(n-4) a(0) = 1,
 *            a(1) = 29, a(2) = 563, a(3) = 9241
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)563;
    if (n == 3) return (seq_t)9241;

    /* Iterate using the 4‑term recurrence. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)29;  /* a(1) */
    seq_t a2 = (seq_t)563; /* a(2) */
    seq_t a3 = (seq_t)9241;/* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)29 * a3;
        next -= (seq_t)278 * a2;
        next += (seq_t)976 * a1;
        next -= (seq_t)1056 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
