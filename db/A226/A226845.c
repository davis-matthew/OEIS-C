/*
 * A226845  Number of n X 1 (0,1,2) arrays of permanents of 2 X 2 subblocks of
 *          some (n+1) X 2 binary array.
 * https://oeis.org/A226845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) + a(n-4) - a(n-5) with
 *            a(1)=3, a(2)=9, a(3)=23, a(4)=57, a(5)=145
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the already known first five terms directly. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)23;
    if (n == 4) return (seq_t)57;
    if (n == 5) return (seq_t)145;

    /* We only need the last five values to apply the recurrence. */
    seq_t a1 = (seq_t)3;   /* a(i-5)  */
    seq_t a2 = (seq_t)9;   /* a(i-4)  */
    seq_t a3 = (seq_t)23;  /* a(i-3)  */
    seq_t a4 = (seq_t)57;  /* a(i-2)  */
    seq_t a5 = (seq_t)145; /* a(i-1)  */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 2*a(i-2) + 2*a(i-3) + a(i-4) - a(i-5) */
        seq_t next = (seq_t)3 * a5
                   - (seq_t)2 * a4
                   + (seq_t)2 * a3
                   + a2
                   - a1;

        /* Shift the window: drop oldest, advance others. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
