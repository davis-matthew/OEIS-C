/*
 * A237560  Number of (n+1) X (1+1) 0..2 arrays with the upper median plus the
 *          lower median of every 2 X 2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A237560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 140*a(n-4) + 632*a(n-6) + 864*a(n-8) for n>9
 *            Initial values: a(1)=81, a(2)=316, a(3)=1256, a(4)=4984,
 *            a(5)=19900, a(6)=78144, a(7)=311512, a(8)=1227648, a(9)=4896848
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init_vals[10] = {
        0ULL,          /* index 0 unused */
        81ULL,         /* a(1) */
        316ULL,        /* a(2) */
        1256ULL,       /* a(3) */
        4984ULL,       /* a(4) */
        19900ULL,      /* a(5) */
        78144ULL,      /* a(6) */
        311512ULL,     /* a(7) */
        1227648ULL,    /* a(8) */
        4896848ULL     /* a(9) */
    };
    /* Use a static array large enough for n up to 10000 */
    static seq_t arr[10001];
    if (n < 1) return 0; /* undefined for offset 1, but safe */
    if (n < 10) return init_vals[n];
    /* compute values up to n iteratively */
    arr[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i < 10)
            arr[i] = init_vals[i];
        else
            arr[i] = 4 * arr[i-2] + 140 * arr[i-4] + 632 * arr[i-6] + 864 * arr[i-8];
    }
    return arr[n];
}
