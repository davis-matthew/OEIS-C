/*
 * A211460  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having one or three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + 5*a(n-2) - 20*a(n-3) - 5*a(n-4)
 *            + 45*a(n-5) - 5*a(n-6) - 40*a(n-7) + 6*a(n-8) + 12*a(n-9)
 *            for n >= 10, with initial values
 *            a(1)=21, a(2)=53, a(3)=121, a(4)=259, a(5)=549,
 *            a(6)=1119, a(7)=2285, a(8)=4575, a(9)=9185.
 * The computation is performed iteratively using a circular buffer
 * of size 9. All arithmetic is done with seq_t, which may be an
 * unsigned integer type or an arbitrary‑precision class.
 */

seq_t a(idx_t n)
{
    /* First nine terms (indices 1..9) */
    static const seq_t init[9] = {
        (seq_t)21, (seq_t)53, (seq_t)121, (seq_t)259,
        (seq_t)549, (seq_t)1119, (seq_t)2285, (seq_t)4575, (seq_t)9185
    };

    if (n <= 9) {
        return init[n - 1];
    }

    /* Buffer holds a(k-9) .. a(k-1) as buf[0] .. buf[8] */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) {
        buf[i] = init[i];          /* buf[0]=a(1), buf[8]=a(9) initially */
    }

    for (idx_t k = 10; k <= n; ++k) {
        /* Positive part of the recurrence */
        seq_t pos = (seq_t)3 * buf[8]          /* a(k-1) */
                 + (seq_t)5 * buf[7]          /* a(k-2) */
                 + (seq_t)45 * buf[4]         /* a(k-5) */
                 + (seq_t)6 * buf[1]          /* a(k-8) */
                 + (seq_t)12 * buf[0];        /* a(k-9) */

        /* Negative part of the recurrence */
        seq_t neg = (seq_t)20 * buf[6]         /* a(k-3) */
                 + (seq_t)5  * buf[5]         /* a(k-4) */
                 + (seq_t)5  * buf[3]         /* a(k-6) */
                 + (seq_t)40 * buf[2];        /* a(k-7) */

        seq_t new_val = pos - neg;            /* result is non‑negative */

        /* Shift buffer left by one (drop a(k-9), move others up) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf[7];
        buf[7] = buf[8];
        buf[8] = new_val;                     /* a(k) now stored at buf[8] */
    }

    return buf[8]; /* a(n) is stored after the final iteration */
}
