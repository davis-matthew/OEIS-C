/*
 * A072905  a(n) is the least k > n such that k*n is a square.
 * https://oeis.org/A072905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = s * (⌊√(n / s)⌋ + 1)^2, where s is the squarefree part of
 *            n. It computes the least k > n such that n·k is a perfect square
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* External types */
extern seq_t a(idx_t n);

/* Integer square root of a 64‑bit value (floor). */
static inline idx_t isqrt_uint64(idx_t v)
{
    /* Initial approximation using long double; then adjust to exact floor. */
    idx_t r = (idx_t)sqrt((long double)v + 0.5L);
    while ((r + 1ULL) * (r + 1ULL) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

/* Least k > n such that n*k is a perfect square. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Compute the squarefree part s of n. */
    idx_t temp = n;
    idx_t s = 1;                /* squarefree part */
    idx_t p = 2;

    while (p * p <= temp) {
        if (temp % p == 0) {
            unsigned int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp & 1) s *= p;   /* include prime if exponent is odd */
        }
        p += (p == 2) ? 1 : 2;        /* iterate 2,3,5,7,… */
    }
    if (temp > 1) {            /* remaining prime factor has odd exponent 1 */
        s *= temp;
    }

    /* n/s is a perfect square. */
    idx_t square_part = n / s;
    idx_t root = isqrt_uint64(square_part);   /* floor sqrt(square_part) */
    idx_t next_root = root + 1;              /* minimal > root */

    /* Compute result in seq_t to avoid overflow. */
    seq_t mult = (seq_t)s * (seq_t)next_root * (seq_t)next_root;
    return mult;
}
