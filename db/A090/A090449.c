/*
 * A090449  Fifth column (m=4) of triangle A090447.
 * https://oeis.org/A090449
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4*(n-1)^3*(n-2)^2*(n-3))/(1!*2!*3!*4!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 17, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 925 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline void mul_factor(seq_t *prod, seq_t factor, int *cnt2, int *cnt3)
{
    /* cancel powers of 2 */
    while (*cnt2 > 0 && factor % 2 == 0)
    {
        factor /= 2;
        (*cnt2)--;
    }

    /* cancel powers of 3 */
    while (*cnt3 > 0 && factor % 3 == 0)
    {
        factor /= 3;
        (*cnt3)--;
    }

    *prod *= factor;
}
/* 1!*2!*3!*4! = 288 = 2^5 * 3^2 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    seq_t prod = 1;
    int   cnt2  = 5;   /* 2  in 288 */
    int   cnt3  = 2;   /* 3  in 288 */

    /* n^4 */
    mul_factor(&prod, n, &cnt2, &cnt3);
    mul_factor(&prod, n, &cnt2, &cnt3);
    mul_factor(&prod, n, &cnt2, &cnt3);
    mul_factor(&prod, n, &cnt2, &cnt3);

    /* (n-1)^3 */
    mul_factor(&prod, n - 1, &cnt2, &cnt3);
    mul_factor(&prod, n - 1, &cnt2, &cnt3);
    mul_factor(&prod, n - 1, &cnt2, &cnt3);

    /* (n-2)^2 */
    mul_factor(&prod, n - 2, &cnt2, &cnt3);
    mul_factor(&prod, n - 2, &cnt2, &cnt3);

    /* (n-3)  */
    mul_factor(&prod, n - 3, &cnt2, &cnt3);

    /* any remaining factors of 2 and 3 */
    while (cnt2 > 0) { prod *= 2; cnt2--; }
    while (cnt3 > 0) { prod *= 3; cnt3--; }

    return prod;
}
