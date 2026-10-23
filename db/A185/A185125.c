/*
 * A185125  Expansion of f(-x, x^5) in powers of x where f(,) is the Ramanujan
 *          general theta function.
 * https://oeis.org/A185125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = coefficient of xⁿ in Σ_{k∈ℤ} (-1)^⌊k/2⌋ · x^{k·(3k+2)}
 *            * (equivalently a(n) = (-1)^⌊k/2⌋ for the unique integer k with
 *            n = 3k²+2k, 0 otherwise)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer square root  floor(sqrt(n)) for 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t bit = 1ULL << 62;          /* highest power of four ≤ n */
    while (bit > n) bit >>= 2;
    uint64_t res = 0;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* compute 1 + 3·n, using 128‑bit to avoid overflow for very large n */
    unsigned __int128 tmp = (unsigned __int128)n * 3 + 1;
    if (tmp > (unsigned __int128)UINT64_MAX) return 0;   /* out of range */
    uint64_t t2 = (uint64_t)tmp;

    uint64_t t = isqrt_u64(t2);
    if ((unsigned __int128)t * t != tmp) return 0;        /* n is not of the required form */

    /* the unique integer k solving n = 3k² + 2k */
    seq_t k;
    uint64_t rm = t % 3;
    if (rm == 1) {
        k = (seq_t)(t - 1) / 3;       /* k ≥ 0 */
    } else if (rm == 2) {
        k = -(seq_t)(t + 1) / 3;      /* k < 0 */
    } else {
        return 0;                     /* should not happen for integer solutions */
    }

    /* floor(k/2) works for negative k with the expression below */
    seq_t floor_k2 = (k - (k & 1)) / 2;

    /* (-1)^floor(k/2) : +1 if floor_k2 even, -1 if odd */
    seq_t sign = ((floor_k2 & 1) ? -1 : 1);
    return sign;
}
