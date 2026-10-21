/*
 * A061579  Reverse one number (0), then two numbers (2,1), then three
 *          (5,4,3), then four (9,8,7,6), etc.
 * https://oeis.org/A061579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(sqrt(2n+1)-1/2) * floor(sqrt(2n+1)+3/2) - n
 *
 * Implementation details:
 * The two floor terms depend on the fractional part of sqrt(2n+1).
 * Let s = ⌊sqrt(M)⌋ where M = 2n+1.
 * If the fractional part < 0.5 then the product is (s-1)(s+1)=s²-1,
 * otherwise it is s(s+2)=s²+2s.
 * We decide which case applies by comparing 4*M with 4*s²+4*s+1
 * (the inequality 4M < 4s²+4s+1 ⇔ fractional part < 0.5).
 * All arithmetic is performed in the seq_t type, using only integral operators.
 */

#include <stdint.h>

/* Integer square root for arbitrary-size unsigned integer 'x'.
 * Returns ⌊sqrt(x)⌋.  Works for seq_t types of any width. */
static inline seq_t isqrt_seq(seq_t x)
{
    if (x < 2) return x;
    seq_t low = 1;
    seq_t high = 1;
    /* find high = 2^k such that high^2 > x */
    while (high <= x / high) {
        high <<= 1;
    }
    low = high >> 1;
    /* binary search in [low, high-1] */
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;   /* high < low, high is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    seq_t M = (seq_t)n * 2 + 1;          /* M = 2n + 1 */
    seq_t s = isqrt_seq(M);            /* s = floor(sqrt(M)) */

    /* Decide if fractional part of sqrt(M) is < 0.5 */
    seq_t left  = 4 * M;
    seq_t middle = 4 * s * s + 4 * s + 1;   /* 4*s^2 + 4*s + 1 */
    seq_t prod;
    if (left < middle) {                /* fractional part < 0.5 */
        prod = (s - 1) * (s + 1);      /* s² - 1 */
    } else {                            /* fractional part ≥ 0.5 */
        prod = s * (s + 2);           /* s² + 2s */
    }
    return prod - (seq_t)n;
}
