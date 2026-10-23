/*
 * A190846  (Squarefree part of (ABC))/C for A=1, C=A+B, as a function of B,
 *          rounded to the nearest integer.
 * https://oeis.org/A190846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = round_to_even( SQF(n*(n+1)) / (n+1) )
 * where SQF(m) is the square‑free part of m (product of distinct primes dividing m).
 * For A = 1, C = A + B = n + 1, and B = n.
 * The rounding rule is nearest integer, ties → even integer.
 */

#include <stdint.h>

/* Return the squarefree part (radical) of a 64‑bit integer. */
static seq_t rad_uint64(uint64_t m)
{
    seq_t r = 1;
    if (m == 0) return r;           /* not used for n >= 1 */

    if ((m & 1ULL) == 0) {
        r *= 2;
        while ((m & 1ULL) == 0) m >>= 1;
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            r *= (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) r *= (seq_t)m;
    return r;
}

seq_t a(idx_t n)
{
    uint64_t B = (uint64_t)n;          /* B = n */
    uint64_t C = B + 1;                /* C = n + 1 */

    seq_t radB = rad_uint64(B);        /* SQF(B) */
    seq_t radC = rad_uint64(C);        /* SQF(C) */

    /* k = C / radC ; always integer since radC | C */
    seq_t k = C / radC;

    /* compute q = radB / k and remainder r = radB % k */
    seq_t q = radB / k;
    seq_t r = radB % k;

    seq_t half = k / 2;                /* floor(k/2) */
    if (r > half) {                    /* fraction > 0.5 */
        return q + 1;
    }
    if (r < half) {                    /* fraction < 0.5 */
        return q;
    }
    /* r == half : exact 0.5 when k even */
    if ((k & 1) == 0) {                /* tie case */
        /* choose the even of q and q+1 */
        if ((q & 1) == 0)
            return q;
        else
            return q + 1;
    }
    /* cannot happen for odd k, just return q */
    return q;
}
