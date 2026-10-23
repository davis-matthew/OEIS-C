/*
 * A188472  a(n) = [6r]-[nr]-[6r-nr], where r=(1+sqrt(5))/2 and []=floor.
 * https://oeis.org/A188472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [6r]-[nr]-[6r‑nr], where r=(1+sqrt(5))/2 and [] means
 *            floor
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for __int128, floor(sqrt(x)) */
static inline __int128 isqrt128(__int128 x) {
    __int128 r = 0;
    __int128 bit = (__int128)1 << 126;          /* highest power of two <= sqrt(max) */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Floor of k * r where r = (1+sqrt(5))/2, k may be negative. */
static inline __int128 floor_phi_int(int64_t k) {
    /* Find the first time values are initialized */
    static int initialized = 0;
    static const int B = 60;                 /* fractional bits */
    static const __int128 SCALE = ((__int128)1 << B);
    static __int128 FRACT;                  /* floor(sqrt(5) * 2^B) */
    static const int SHIFT = B + 1;          /* divide by 2^(B+1) */

    if (!initialized) {
        __int128 tmp = (__int128)5 << (2 * B);   /* 5 * 2^(2B) */
        FRACT = isqrt128(tmp);
        initialized = 1;
    }

    __int128 M = SCALE + FRACT;            /* 2^B + sqrt(5)*2^B ≈ φ * 2^B */
    __int128 num = (__int128)k * M;        /* k * φ * 2^B  (signed) */
    return num >> SHIFT;                   /* floor division by 2^(B+1) */
}

/* Sequence function as required */
seq_t a(idx_t n) {
    /* Convert n to signed integer for negative arithmetic */
    int64_t k1 = (int64_t)n;
    int64_t k2 = (int64_t)6 - k1;               /* (6 - n) may be negative */

    __int128 f1 = floor_phi_int(k1);
    __int128 f2 = floor_phi_int(k2);

    /* floor(6r) = 9, so a(n) = 9 - (f1 + f2) */
    int64_t res = 9 - (int64_t)(f1 + f2);      /* result is 0 or 1 */

    return (seq_t)res;
}
