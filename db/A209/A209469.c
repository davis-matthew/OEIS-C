/*
 * A209469  Number of n X 1 0..2 arrays with every element value z a city
 *          block distance of exactly z from another element value z.
 * https://oeis.org/A209469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + 4*a(n-5) - 3*a(n-6) +
 *            3*a(n-7) + a(n-8) - 2*a(n-9) + a(n-10) - a(n-11) (for n ≥ 12),
 *            with initial terms a(1)=1, a(2)=2, a(3)=5, a(4)=10, a(5)=21,
 *            a(6)=46, a(7)=97, a(8)=207, a(9)=437, a(10)=920, a(11)=1941
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* List of the first 11 terms (a(1) … a(11)) */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)10, (seq_t)21,
        (seq_t)46, (seq_t)97, (seq_t)207, (seq_t)437,
        (seq_t)920, (seq_t)1941
    };

    if (n == 0) return (seq_t)0;          /* undefined, but return 0 */
    if (n <= 11) return init[n - 1];

    /* Keep the last 11 computed values in a circular buffer. */
    seq_t last[11];
    for (idx_t i = 0; i < 11; ++i) last[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        seq_t new_val =
              (seq_t)3 * last[10]   /* a(i-1)  */
            - (seq_t)2 * last[9]    /* a(i-2)  */
            -          last[7]     /* a(i-4)  */
            + (seq_t)4 * last[6]   /* a(i-5)  */
            - (seq_t)3 * last[5]   /* a(i-6)  */
            + (seq_t)3 * last[4]   /* a(i-7)  */
            +          last[3]     /* a(i-8)  */
            - (seq_t)2 * last[2]   /* a(i-9)  */
            +          last[1]     /* a(i-10) */
            -          last[0];    /* a(i-11) */

        /* Shift buffer: last[0] <- last[1], …, last[9] <- last[10] */
        for (idx_t j = 0; j < 10; ++j) last[j] = last[j + 1];
        last[10] = new_val;
    }

    return last[10];   /* a(n) is now in last[10] */
}
