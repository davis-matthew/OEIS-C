/*
 * A188017  [nr]-[nr-kr]-[kr], where r=(1+sqrt(5))/2, k=6, [ ]=floor.
 * https://oeis.org/A188017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n r⌋ – ⌊(n–6) r⌋ – ⌊6 r⌋ where r = (1+√5)/2. The formula
 *            is evaluated by integer arithmetic only
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned __uint128_t. */
static inline __uint128_t isqrt_uint128(__uint128_t x)
{
    __uint128_t r = 0;
    __uint128_t bit = (__uint128_t)1 << 126;   /* highest admissible bit */
    while (bit) {
        __uint128_t temp = r + bit;
        if (temp != 0 && temp <= x / temp) r = temp;
        bit >>= 1;
    }
    return r;
}

/* ⌊n·φ⌋  for n ≥ 0.
   Uses the identity:  ⌊n·φ⌋ = ⌊(n + ⌊√(5·n²)⌋)/2⌋. */
static inline seq_t floor_n_phi(idx_t n)
{
    __uint128_t mul = (__uint128_t)5 * n * n;
    __uint128_t s   = isqrt_uint128(mul);
    return (seq_t)(( (__uint128_t)n + s ) >> 1);
}

/* ⌊m·φ⌋  for signed integer m (m may be negative). */
static inline int64_t floor_phi_int64(int64_t m)
{
    if (m >= 0) {
        seq_t val = floor_n_phi((idx_t)m);
        return (int64_t)val;
    } else {
        int64_t t   = -m;                    /* t > 0 */
        seq_t val   = floor_n_phi((idx_t)t); /* floor(t·φ) */
        return -((int64_t)val + 1);          /* ceil(t·φ) = val+1, so floor(m·φ) = -(val+1) */
    }
}

/* Sequence A188017 */
seq_t a(idx_t n)
{
    /* The value for ⌊6 r⌋ is constant: ⌊6 (1+√5)/2⌋ = ⌊9.708…⌋ = 9 */
    const seq_t floor_6r = 9;

    int64_t fn   = floor_phi_int64((int64_t)n);           /* ⌊n r⌋ */
    int64_t fneg = floor_phi_int64((int64_t)n - 6);       /* ⌊(n–6) r⌋ */
    int64_t res  = fn - fneg - (int64_t)floor_6r;         /* a(n) */

    return (seq_t)res;
}
