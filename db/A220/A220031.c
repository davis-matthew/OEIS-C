/*
 * A220031  Number of n X 7 arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 n X 7 array.
 * https://oeis.org/A220031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) (n>9) Initial values: a(1)=7, a(2)=18,
 *            a(3)=55, a(4)=137, a(5)=302, a(6)=613, a(7)=1165, a(8)=2094,
 *            a(9)=3587
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base terms a[1]..a[9] (indexing from 1) */
    static const seq_t base[9] = {
        (seq_t)7ULL,
        (seq_t)18ULL,
        (seq_t)55ULL,
        (seq_t)137ULL,
        (seq_t)302ULL,
        (seq_t)613ULL,
        (seq_t)1165ULL,
        (seq_t)2094ULL,
        (seq_t)3587ULL
    };

    if (n == 0)
        return 0;                 /* undefined case, safe fallback */

    if (n <= 9)
        return base[n - 1];        /* direct lookup for the first 9 terms */

    /* last[0] = a(n-7), ..., last[6] = a(n-1) for the current n */
    seq_t last[7] = {
        base[2],   /* a(3)   */
        base[3],   /* a(4)   */
        base[4],   /* a(5)   */
        base[5],   /* a(6)   */
        base[6],   /* a(7)   */
        base[7],   /* a(8)   */
        base[8]    /* a(9)   */
    };

    for (idx_t cur = 10; cur <= n; ++cur)
    {
        /* positive contributions: 7*a(n-1) + 35*a(n-3) + 21*a(n-5) + a(n-7) */
        seq_t positive = (seq_t)7ULL * last[6];
        positive += (seq_t)35ULL * last[4];
        positive += (seq_t)21ULL * last[2];
        positive += last[0];

        /* negative contributions: 21*a(n-2) + 35*a(n-4) + 7*a(n-6) */
        seq_t negative = (seq_t)21ULL * last[5];
        negative += (seq_t)35ULL * last[3];
        negative += (seq_t)7ULL  * last[1];

        seq_t next = positive - negative;

        /* shift the window left by one position */
        for (int i = 0; i < 6; ++i)
            last[i] = last[i + 1];
        last[6] = next;
    }

    return last[6];  /* a(n) */
}
