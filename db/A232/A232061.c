/*
 * A232061  Number of nX4 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or vertically, with no adjacent values
 *          equal.
 * https://oeis.org/A232061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-4) - 3*a(n-5) + a(n-7) +
 *            3*a(n-8) - a(n-11); Initial values (n = 1 … 11): a(1) = 6, a(2)
 *            = 30, a(3) = 130, a(4) = 534, a(5) = 2230, a(6) = 9310, a(7) =
 *            38870, a(8) = 162278, a(9) = 677510, a(10) = 2828606, a(11) =
 *            11809446
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset (not used in tests). */
    if (n == 0)
        return (seq_t)0;

    /* For n ≤ 11 we can answer directly. */
    static const seq_t init[11] =
    {
        (seq_t)6,        /* a(1)  */
        (seq_t)30,       /* a(2)  */
        (seq_t)130,      /* a(3)  */
        (seq_t)534,      /* a(4)  */
        (seq_t)2230,     /* a(5)  */
        (seq_t)9310,     /* a(6)  */
        (seq_t)38870,    /* a(7)  */
        (seq_t)162278,   /* a(8)  */
        (seq_t)677510,   /* a(9)  */
        (seq_t)2828606,  /* a(10) */
        (seq_t)11809446  /* a(11) */
    };

    if (n <= 11)
        return init[n-1];

    /* Array to hold the last 11 values.
       prev[0] = a(k-11), prev[10] = a(k-1). */
    seq_t prev[11];
    for (idx_t i = 0; i < 11; ++i)
        prev[i] = init[i];

    /* Compute a(k) for k = 12 … n. */
    for (idx_t k = 12; k <= n; ++k)
    {
        /* Use the correct offsets:
           a(k-1) -> prev[10]
           a(k-2) -> prev[9]
           a(k-4) -> prev[7]
           a(k-5) -> prev[6]
           a(k-7) -> prev[4]
           a(k-8) -> prev[3]
           a(k-11) -> prev[0]          */
        seq_t val = 4 * prev[10]
                  + prev[9]
                  - 4 * prev[7]
                  - 3 * prev[6]
                  + prev[4]
                  + 3 * prev[3]
                  - prev[0];

        /* Shift the window to make room for the new value. */
        for (idx_t i = 0; i < 10; ++i)
            prev[i] = prev[i + 1];
        prev[10] = val;
    }

    return prev[10];
}
