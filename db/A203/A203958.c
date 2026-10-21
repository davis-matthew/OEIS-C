/*
 * A203958  Number of (n+1) X 2 0..2 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that rows
 *          of b(i,j) and columns of c(i,j) are lexicographically
 *          nondecreasing.
 * https://oeis.org/A203958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -40*a(n-2) +82*a(n-3) -91*a(n-4) +52*a(n-5)
 *            -12*a(n-6). Initial values: a(1)=32, a(2)=121, a(3)=447,
 *            a(4)=1579, a(5)=5352, a(6)=17559
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (outside the OEIS offset). */
    if (n == 0) return 0;

    /* Handle the first six terms directly. */
    switch ((int)n) {
        case 1: return (seq_t)32;
        case 2: return (seq_t)121;
        case 3: return (seq_t)447;
        case 4: return (seq_t)1579;
        case 5: return (seq_t)5352;
        case 6: return (seq_t)17559;
        default:
            /* Use a sliding window of the six most recent values. */
            seq_t a1 = (seq_t)32;   /* a(i-6) */
            seq_t a2 = (seq_t)121;  /* a(i-5) */
            seq_t a3 = (seq_t)447;  /* a(i-4) */
            seq_t a4 = (seq_t)1579; /* a(i-3) */
            seq_t a5 = (seq_t)5352; /* a(i-2) */
            seq_t a6 = (seq_t)17559;/* a(i-1) */
            seq_t next = 0;

            for (idx_t i = 7; i <= n; ++i) {
                /* Compute the positive and negative parts separately
                   to guarantee non‑negative subtraction for unsigned types. */
                seq_t pos = a6 * (seq_t)10 + a4 * (seq_t)82 + a2 * (seq_t)52;
                seq_t neg = a5 * (seq_t)40 + a3 * (seq_t)91 + a1 * (seq_t)12;
                next = pos - neg;

                /* Shift the window for the next iteration. */
                a1 = a2;
                a2 = a3;
                a3 = a4;
                a4 = a5;
                a5 = a6;
                a6 = next;
            }
            return next;
    }
}
