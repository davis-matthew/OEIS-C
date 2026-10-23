/*
 * A014796  Squares of even tetrahedral numbers (A015220).
 * https://oeis.org/A014796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(k)^2 where k = n + ceil(n/3) and T(k) = k(k+1)(k+2)/6
 *            (the tetrahedral number). The sequence uses every k except those
 *            congruent to 1 (mod 4); the mapping k = n + ceil(n/3) yields
 *            exactly those k in order
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Map index n to the k giving the n‑th even tetrahedral number. */
    idx_t k = n + (n + 2) / 3;      /* ceil(n/3) = (n+2)/3 */

    /* Compute T(k) = k(k+1)(k+2)/6 safely by cancelling 2 and 3 early. */
    seq_t p = (seq_t)k;
    seq_t q = (seq_t)(k + 1);
    seq_t r = (seq_t)(k + 2);

    /* Divide by 2. One of p, q, r must be even. */
    if ((p & 1) == 0)
        p /= 2;
    else if ((q & 1) == 0)
        q /= 2;
    else
        r /= 2;

    /* Divide by 3. One of p, q, r must be divisible by 3. */
    if (p % 3 == 0)
        p /= 3;
    else if (q % 3 == 0)
        q /= 3;
    else
        r /= 3;

    /* Now p*q*r is T(k). */
    seq_t t = p;
    t = t * q;
    t = t * r;

    /* Return the square of the even tetrahedral number. */
    return t * t;
}
