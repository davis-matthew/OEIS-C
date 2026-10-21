/*
 * A210302  Number of (n+1) X 2 0..3 arrays containing all values 0..3 with
 *          every 2 X 2 subblock having one or three distinct values, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A210302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 57*a(n-2) + 41*a(n-3) + 10*a(n-4) - 8*a(n-5)
 *            Empirical initial values: a(1) = 0 a(2) = 29 a(3) = 414 a(4) =
 *            4553 a(5) = 45890 This recurrence holds for all n ≥ 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)29;
    if (n == 3) return (seq_t)414;
    if (n == 4) return (seq_t)4553;
    if (n == 5) return (seq_t)45890;

    /* Use a sliding window of the last five computed values. */
    seq_t v[5];
    v[0] = (seq_t)0;      /* a(1)  */
    v[1] = (seq_t)29;     /* a(2)  */
    v[2] = (seq_t)414;    /* a(3)  */
    v[3] = (seq_t)4553;   /* a(4)  */
    v[4] = (seq_t)45890;  /* a(5)  */

    /* Compute terms iteratively up to n. */
    for (idx_t i = 6; i <= n; ++i)
    {
        /* Recurrence using only the five stored values. */
        seq_t pos = 15 * v[4] + 41 * v[2] + 10 * v[1];
        seq_t neg = 57 * v[3] + 8  * v[0];
        seq_t next = pos - neg;              /* guaranteed non‑negative */

        /* Slide the window for the next step. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }

    return v[4];
}
