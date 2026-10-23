/*
 * A035179  a(n) = Sum_{d|n} Kronecker(-11, d).
 * https://oeis.org/A035179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-11,d)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the multiplicative definition:  */
/*   a(1) = 1 */
/*   For a prime power p^e,  */
/*      if p == 11                     -> a(p^e) = 1,         */
/*      else if p ≡ 1,3,4,5,9 (mod 11) -> a(p^e) = e + 1,     */
/*      else (p ≡ 2,6,7,8,10 (mod 11)) -> a(p^e) = (e even) ? 1 : 0.  */
/*   Then a(n) = ∏ a(p^e) over all prime powers dividing n. */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;                /* a(1) = 1 */

    seq_t result = (seq_t)1;                   /* multiplicative product */
    uint64_t m = (uint64_t)n;                  /* work with 64‑bit unsigned */

    /* factorisation by trial division */
    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p != 0) continue;             /* not a divisor */

        /* exponent of current prime */
        uint64_t e = 0;
        while (m % p == 0) { m /= p; ++e; }

        /* compute a(p^e) according to residue modulo 11 */
        uint64_t ap = 1;                       /* default for p = 11 */
        if (p != 11)
        {
            uint64_t r = p % 11;
            if (r == 1 || r == 3 || r == 4 || r == 5 || r == 9)
                ap = e + 1;                    /* e+1 for these residues */
            else if (r == 2 || r == 6 || r == 7 || r == 8 || r == 10)
                ap = (e & 1) ? 0 : 1;          /* 0 if e odd, 1 if e even */
            /* other residues cannot occur (p>11) */
        }

        if (ap == 0) return (seq_t)0;          /* product becomes zero */
        result = result * (seq_t)ap;           /* multiply in generic type */
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1)
    {
        uint64_t p = m;                        /* exponent is 1 */
        uint64_t ap;

        if (p == 11) ap = 1;
        else
        {
            uint64_t r = p % 11;
            if (r == 1 || r == 3 || r == 4 || r == 5 || r == 9)
                ap = 2;                        /* e+1 with e=1 */
            else if (r == 2 || r == 6 || r == 7 || r == 8 || r == 10)
                ap = 0;                        /* e odd -> 0 */
            else ap = 1;
        }

        if (ap == 0) return (seq_t)0;
        result = result * (seq_t)ap;
    }

    return result;
}
