/*
 * A081078  a(n) = Lucas(4n) - 3, or Lucas(2n-1)*Lucas(2n+1).
 * https://oeis.org/A081078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4n) – 3 Recurrence used: a(n) = 8*a(n‑1) – 8*a(n‑2)
 *            + a(n‑3) Initial values (offset 1): a(1) = 4 a(2) = 44 a(3) =
 *            319
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety – sequence is defined only for n ≥ 1 */
    if (n < 1)
        return 0;

    /* Base cases */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)44;
    if (n == 3) return (seq_t)319;

    /* Compute a(n) iteratively using the recurrence. */
    seq_t a0 = (seq_t)4;   /* a(n‑3) */
    seq_t a1 = (seq_t)44;  /* a(n‑2) */
    seq_t a2 = (seq_t)319; /* a(n‑1) */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        seq_t val = a2 * (seq_t)8;      /* 8*a(n‑1) */
        val -= a1 * (seq_t)8;          /* –8*a(n‑2) */
        val += a0;                     /* + a(n‑3) */

        /* Shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = val;
    }
    return a2;
}
