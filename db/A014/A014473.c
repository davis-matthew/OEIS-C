/*
 * A014473  Pascal's triangle - 1: Triangle read by rows: T(n, k) = A007318(n,
 *          k) - 1.
 * https://oeis.org/A014473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,k)-1, where r = floor((sqrt(8*n+1)-1)/2), k =
 *            n - r*(r+1)/2 (T(n,k)=C(n,k)-1 read by rows)
 * Range:     u8 n <= 69, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square‑root of an unsigned __int128, floor value. */
static idx_t sqrt_uint128(unsigned __int128 x)
{
    long double d = sqrtl((long double)x);
    unsigned __int128 s = (unsigned __int128)d;
    /* Correct possible rounding errors */
    while ((s + 1) * (s + 1) <= x) ++s;
    while (s * s > x) --s;
    return (idx_t)s;
}

/* Return a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Determine row r and column k. */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t s = sqrt_uint128(tmp);
    idx_t r = (s - 1) / 2;
    idx_t start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - start;

    /* Use the smaller of k and r-k for efficiency. */
    idx_t kk = k;
    if (kk > r - kk) kk = r - kk;

    /* Compute binomial C(r,kk) - 1, taking care of overflow. */
    if (sizeof(seq_t) <= 8) {
        /* seq_t is a 64‑bit type – use 128‑bit intermediates. */
        seq_t res = 1;
        for (idx_t i = 1; i <= kk; ++i) {
            idx_t num = r - kk + i;  /* numerator */
            idx_t den = i;           /* denominator */

            unsigned __int128 temp = (unsigned __int128)res * (unsigned __int128)num;
            temp /= den;
            res = (seq_t)temp;
        }
        return res - (seq_t)1;
    } else {
        /* seq_t is an arbitrary‑precision type – direct operations. */
        seq_t res = 1;
        for (idx_t i = 1; i <= kk; ++i) {
            seq_t num = (seq_t)(r - kk + i);
            seq_t den = (seq_t)i;
            res = res * num;
            res = res / den;
        }
        return res - (seq_t)1;
    }
}
