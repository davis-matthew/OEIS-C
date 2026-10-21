/*
 * A219472  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..3 2 X n array.
 * https://oeis.org/A219472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            12; Initial values: a(1)=10, a(2)=23, a(3)=68, a(4)=211,
 *            a(5)=547, a(6)=1248, a(7)=2663, a(8)=5432, a(9)=10666,
 *            a(10)=20206, a(11)=36984, a(12)=65512
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 52, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, not used by the problem */
    static const seq_t init[12] = {
        10ull, 23ull, 68ull, 211ull, 547ull,
        1248ull, 2663ull, 5432ull, 10666ull,
        20206ull, 36984ull, 65512ull
    };
    if (n <= 12) return init[n - 1];

    /* last nine terms needed for the recurrence.
       For n = 13 the required previous terms are a(4)..a(12) */
    seq_t t[9];
    for (idx_t k = 0; k < 9; ++k) t[k] = init[3 + k];   /* a(4) to a(12) */

    seq_t next;
    for (idx_t i = 13; i <= n; ++i) {
        next = 9 * t[8]      /* a(n-1) */
             - 36 * t[7]     /* a(n-2) */
             + 84 * t[6]     /* a(n-3) */
             - 126 * t[5]    /* a(n-4) */
             + 126 * t[4]    /* a(n-5) */
             - 84 * t[3]     /* a(n-6) */
             + 36 * t[2]     /* a(n-7) */
             - 9  * t[1]     /* a(n-8) */
             + t[0];         /* a(n-9) */

        /* shift the window: drop the oldest, add the new */
        for (idx_t k = 0; k < 8; ++k) t[k] = t[k + 1];
        t[8] = next;
    }
    return t[8];
}
