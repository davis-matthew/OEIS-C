/*
 * A156685  Number of primitive Pythagorean triples A^2 + B^2 = C^2 with 0 < A
 *          < B < C and gcd(A,B)=1 that have a hypotenuse C that is less than
 *          or equal to n.
 * https://oeis.org/A156685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = number of primitive Pythagorean triples A^2 + B^2 = C^2
 *            with 0 < A < B < C, gcd(A,B)=1, and hypotenuse C ≤ n
 * Range:     u8 n <= 1608, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 ≤ x ≤ UINT64_MAX */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;            /* highest power of 2^2 ≤ x */
    while (x < bit)
        bit >>= 2;

    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* Euclidean gcd for 64‑bit unsigned integers */
static idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 5)                /* smallest hypotenuse of a primitive triple is 5 */
        return 0;

    seq_t count = 0;

    /* m > n > 0 are the Euclid parameters.
       We iterate m up to sqrt(n) because m^2 must be ≤ n. */
    idx_t m_max = isqrt_u64(n);

    for (idx_t m = 2; m <= m_max; ++m) {
        /* For a fixed m, n^2 ≤ n - m^2 to keep c = m^2 + n^2 ≤ n. */
        idx_t remain = n - m * m;
        if (remain == 0)
            continue;                  /* no n possible */

        idx_t max_n = isqrt_u64(remain);

        /* Only consider n < m to avoid duplicates and negative a. */
        if (max_n > m - 1)
            max_n = m - 1;

        for (idx_t nb = 1; nb <= max_n; ++nb) {
            /* m and n must have opposite parity to generate a primitive triple. */
            if (((m ^ nb) & 1) == 0)
                continue;
            /* gcd(m,n) must be 1. */
            if (gcd_u64(m, nb) != 1)
                continue;
            /* (m,n) yields a primitive triple with c = m^2 + n^2 ≤ n. */
            ++count;
        }
    }

    return count;
}
