/*
 * A133044  Area of the spiral of equilateral triangles with side lengths
 *          which follow the Padovan sequence, divided by the area of the
 *          initial triangle.
 * https://oeis.org/A133044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6)
 *            Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=11, a(6)=20
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* unused – safety guard */

    /* cache[0] is unused; cache[1] .. cache[10000] store a(1) .. a(10000) */
    static seq_t cache[10001];
    static idx_t len = 0;

    if (len == 0) {
        cache[1] = 1;   /* a(1) */
        cache[2] = 2;   /* a(2) */
        cache[3] = 3;   /* a(3) */
        cache[4] = 7;   /* a(4) */
        cache[5] = 11;  /* a(5) */
        cache[6] = 20;  /* a(6) */
        len = 6;
    }

    if (n <= len)
        return cache[n];

    for (idx_t i = len + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                 - cache[i-4] + cache[i-5] - cache[i-6];
    }
    len = n;
    return cache[n];
}
