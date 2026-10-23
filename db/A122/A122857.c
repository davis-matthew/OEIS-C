/*
 * A122857  Expansion of (phi(q)^2 + phi(q^3)^2) / 2 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A122857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * b(n) * * b(n) is multiplicative with * b(2^e) =
 *            b(3^e) = 1 * for primes p ≡ 1,5 (mod 12): b(p^e) = e + 1 * for
 *            primes p ≡ 7,11 (mod 12): b(p^e) = 1 if e is even, 0 if e is odd
 *            * * The sequence is
 *            1,2,2,2,2,4,2,0,2,2,4,0,2,4,0,4,2,4,2,0,4,0,0,0,... * * This
 *            implementation factors n by t...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    uint64_t tmp = (uint64_t)n;      /* work on native 64‑bit integer */
    seq_t     prod = (seq_t)1;       /* product of local factors */

    /* Trial division */
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            if (p == 2 || p == 3)
            {
                /* factors 2^e and 3^e contribute a factor of 1 – nothing to do */
            }
            else if ((p % 12 == 1) || (p % 12 == 5))
            {
                /* b(p^e) = e + 1 */
                prod = prod * (seq_t)(e + 1);
            }
            else            /* p % 12 == 7 or 11 */
            {
                /* b(p^e) = 1 if e even, 0 if e odd */
                if (e & 1)    /* odd exponent → zero factor → total zero */
                    return (seq_t)0;
                /* even exponent → factor 1 → nothing to multiply */
            }
        }
    }

    /* Remaining prime factor > sqrt(n) (if any) */
    if (tmp > 1)
    {
        uint64_t p = tmp;          /* exponent is 1 */
        if (p != 2 && p != 3)
        {
            if ((p % 12 == 1) || (p % 12 == 5))
            {
                prod = prod * (seq_t)2;   /* e = 1 → e + 1 = 2 */
            }
            else            /* p % 12 == 7 or 11, exponent 1 odd */
            {
                return (seq_t)0;
            }
        }
    }

    /* a(n) = 2 * b(n) */
    return prod * (seq_t)2;
}
