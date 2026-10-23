/*
 * A172325  Floor(n*(sqrt(7)+sqrt(3))).
 * https://oeis.org/A172325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(n*(sqrt(7)+sqrt(3)))
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 128‑bit unsigned values.
 * Returns floor(sqrt(x)).  Binary search is used; it runs in at most
 * 128 iterations (far below the limits for the problem). */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)   lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;  /* floor(sqrt(x)) */
}

/* Parameters for the rational approximation of sqrt(7)+sqrt(3).
 * Q is the denominator; P/Q is the best rational approximation that
 * is less than the true constant.  The chosen Q gives an error
 * far smaller than 1/n for any n up to 10 000, so floor(n*P/Q) equals
 * floor(n*(sqrt(7)+sqrt(3))) for all required n. */
static const unsigned long long Q = 1ULL << 60;   /* 1.1529e18 */

/* Lazy initialization of the numerator P of the approximation. */
static bool init_done = false;
static unsigned __int128 P;   /* numerator */

/* Compute the numerator and mark initialization as done. */
static void init_approximation(void)
{
    unsigned __int128 s7 = isqrt128((unsigned __int128)7 * Q * Q);
    unsigned __int128 s3 = isqrt128((unsigned __int128)3 * Q * Q);
    P = s7 + s3;
    init_done = true;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (!init_done) init_approximation();

    /* compute floor(n * P / Q) using 128‑bit arithmetic. */
    unsigned __int128 prod = (unsigned __int128)n * P;
    unsigned __int128 res  = prod / Q;

    return (seq_t)res;
}
