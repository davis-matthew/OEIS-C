/*
 * A166522  a(n) = 7*n - a(n-1), with a(1) = 1.
 * https://oeis.org/A166522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(14*n + 7 + 17*(-1)^n)
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(1)=1, a(n)=7*n-a(n-1) for n≥2 */

seq_t a(idx_t n)
{
    /* 14*n fits in idx_t for all needed n (≤10K). */
    idx_t tmp = 14 * n;
    seq_t num;

    if (n & 1ull)            /* n is odd: subtract 17         */
        num = (seq_t)(tmp + 7 - 17);
    else                     /* n is even: add 17           */
        num = (seq_t)(tmp + 7 + 17);

    return num / 4;
}
