/*
 * A088557  Least even leg of primitive Pythagorean triangles with odd leg
 *          2n+1.
 * https://oeis.org/A088557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ 2*m*k | (m-k)(m+k) = 2n+1 , gcd(m,k)=1 } (m>k>0, m-k
 *            odd)
 * Range:     u8 n <= 10, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for 64‑bit unsigned integers */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Sequence starts at n=1; undefined for n=0 – return 0. */
    if (n == 0) return 0;

    /* Odd leg of the triple: L = 2n + 1.  Convert to 64‑bit integer to avoid overflow. */
    uint64_t L = (uint64_t)(2 * (seq_t)n + (seq_t)1);
    seq_t best = 0;                 /* will hold the minimal even leg */

    /* Loop over divisors u of L with u <= sqrt(L) */
    for (uint64_t u = 1; u * u <= L; ++u) {
        if (L % u != 0) continue;            /* u is not a divisor */

        uint64_t v = L / u;                   /* u * v == L */

        if (u == v) continue;                /* would give k = 0 */

        /* u and v are odd, so (u+v) and (v-u) are even; m and k are integers. */
        uint64_t m = (u + v) >> 1;            /* (u+v)/2 */
        uint64_t k = (v - u) >> 1;            /* (v-u)/2 */

        /* Primitive condition: gcd(m, k) == 1 */
        if (gcd_u64(m, k) != 1) continue;

        uint64_t even_leg = 2ULL * m * k;     /* 2*m*k */
        seq_t even_val = (seq_t)even_leg;

        if (best == 0 || even_val < best) best = even_val;
    }

    return best;
}
