/*
 * A213588  Principal diagonal of the convolution array A213587.
 * https://oeis.org/A213588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A213588:   a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 5*a(n-4) - a(n-5)
   with a(1)=1, a(2)=7, a(3)=27, a(4)=96, a(5)=315 */
seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)96;
    if (n == 5) return (seq_t)315;

    /* Initialise the last five values:
       arr[0] = a(k-1), arr[1] = a(k-2), arr[2] = a(k-3),
       arr[3] = a(k-4), arr[4] = a(k-5) for k=6 (i.e. current k index).
       For k=6, these are a5, a4, a3, a2, a1 respectively. */
    seq_t arr[5] = {(seq_t)315, (seq_t)96, (seq_t)27,
                    (seq_t)7,   (seq_t)1};

    seq_t val = 0;
    for (idx_t k = 6; k <= n; ++k) {
        /* Compute a(k) from the recurrence */
        val = (seq_t)5 * arr[0]          /* 5*a(k-1) */
            - (seq_t)5 * arr[1]          /* -5*a(k-2) */
            - (seq_t)5 * arr[2]          /* -5*a(k-3) */
            + (seq_t)5 * arr[3]          /* +5*a(k-4) */
            - arr[4];                   /* -a(k-5) */

        /* Shift the history: drop a(k-5), keep the last five values */
        arr[4] = arr[3];
        arr[3] = arr[2];
        arr[2] = arr[1];
        arr[1] = arr[0];
        arr[0] = val;
    }
    return val;
}
