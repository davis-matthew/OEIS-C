/*
 * A233490  Number of (n+2) X (1+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally or antidiagonally downwards.
 * https://oeis.org/A233490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) + 2*a(n-2) - 21*a(n-3) + 3*a(n-4) Initial
 *            values: a(1)=16928, a(2)=424009, a(3)=10619963, a(4)=265993633
 *            The recurrence holds for all n ≥ 5. This implementation
 *            evaluates the recurrence iteratively, avoiding storage growth
 *            and remaining well under the 2‑second limit for n ≈ 10 000
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16928;
    if (n == 2) return (seq_t)424009;
    if (n == 3) return (seq_t)10619963;
    if (n == 4) return (seq_t)265993633;

    /* ring buffer holding the last four terms: w[0]=a(n-4), w[1]=a(n-3),
       w[2]=a(n-2), w[3]=a(n-1) */
    seq_t w[4] = {(seq_t)16928, (seq_t)424009, (seq_t)10619963, (seq_t)265993633};

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t new_val = w[3] * (seq_t)25
                       + w[2] * (seq_t)2
                       - w[1] * (seq_t)21
                       + w[0] * (seq_t)3;

        /* Shift the buffer forward: drop a(i-5), make room for new_val */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = new_val;
    }

    return w[3];  /* a(n) after the loop finishes */
}
