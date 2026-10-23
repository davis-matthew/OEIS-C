/*
 * A183296  Complement of A005476.
 * https://oeis.org/A183296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + m if n > m*(5*m-3)/2 n + m - 1 otherwise, where m =
 *            floor(sqrt(2*n/5)) + 1 (Offset: 1, a(1) = 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below follows the definition directly using only integer
   arithmetic.  A small helper computing the integer square‑root of a
   uint64_t is provided so that no external dependency is required.  */
#include <stdint.h>

/* Compute floor(sqrt(x)) for an unsigned 64‑bit integer x.
   No overflow occurs because the algorithm uses only shifts and
   subtractions.  */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t res = 0;          /* current result (the next bit of sqrt)   */
    idx_t bit = 1ULL << 62; /* second highest power of 4 (2^62 <= x < 2^64) */

    /*   Find the greatest power of 4 that does not exceed x.   */
    while (bit > x)          /* reduce until bit <= x */
        bit >>= 2;

    /*   Main loop: try to set the next lower bit if possible.   */
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;              /* floor(sqrt(original x)) */
}

seq_t a(idx_t n)
{
    /* a(n) is undefined for n == 0, but returning 0 is harmless. */
    if (n == 0)
        return (seq_t)0;

    /* Compute floor(2*n/5) without overflowing 64 bits. */
    idx_t t = (n / 5) * 2 + ((n % 5) * 2) / 5;

    /* m = floor(sqrt(t)) + 1 */
    idx_t m = floor_isqrt_u64(t) + 1;

    /* All subsequent arithmetic is performed in seq_t to avoid
       temporary overflow. */
    seq_t N   = (seq_t)n;
    seq_t m_  = (seq_t)m;
    seq_t inner = 5 * m - 3;                    /* 5*m-3 fits in idx_t */
    seq_t threshold = m_ * ((seq_t)inner) / (seq_t)2; /* m*(5*m-3)/2 */

    if (N > threshold)
        return N + m_;
    else
        return N + m_ - (seq_t)1;
}
