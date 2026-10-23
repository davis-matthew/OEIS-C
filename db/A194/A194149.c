/*
 * A194149  Sum_{floor(j*(5-sqrt(3))/2) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for (5-sqrt(3))/2.
 * https://oeis.org/A194149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{j=1}^{n} \left\lfloor \dfrac{j\,(5-\sqrt{3})}{2}
 *            \right\rfloor
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 128‑bit numbers */
static inline uint64_t isqrt_uint128(unsigned __int128 n) {
    unsigned __int128 l = 0, r = (unsigned __int128)1 << 64;   /* r > sqrt(n) */
    while (l < r) {
        unsigned __int128 m = (l + r) >> 1;
        unsigned __int128 msq = m * m;
        if (msq <= n)
            l = m + 1;
        else
            r = m;
    }
    return (uint64_t)(l - 1);
}

/* Convert an idx_t to seq_t before any arithmetic, as required. */
seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        /* s = floor(j * sqrt(3))  =  floor( sqrt(3 * j^2) ) */
        unsigned long long jj = j;
        unsigned __int128 prod = (unsigned __int128)jj * jj * (unsigned __int128)3;
        uint64_t s = isqrt_uint128(prod);
        seq_t s_seq = (seq_t)s;

        seq_t j_seq = (seq_t)jj;
        seq_t t = (seq_t)5 * j_seq - s_seq;   /* t = 5*j - floor(j*sqrt(3)) */

        /* floor(t / 2) with adjustment for even t */
        if (t & 1) {                         /* odd */
            sum += t >> 1;
        } else {                             /* even */
            sum += (t >> 1) - 1;
        }
    }
    return sum;
}
