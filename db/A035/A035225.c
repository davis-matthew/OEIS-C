/*
 * A035225  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = 43.
 * https://oeis.org/A035225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(43, d) (Kronecker symbol)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function follows the multiplicative description:
   a(1) = 1,
   a(43^e) = 1,
   a(p^e) = e + 1           if Kronecker(43, p) =  1,
   a(p^e) = (1 + (-1)^e)/2  if Kronecker(43, p) = -1. */
#include <stdint.h>
#include <stdbool.h>

/* Quadratic residues modulo 43 – 1 if r is a residue, 0 otherwise. */
static const unsigned char residue[43] = {
    0, /* 0  */
    1, /* 1  */
    0, /* 2  */
    0, /* 3  */
    1, /* 4  */
    0, /* 5  */
    1, /* 6  */
    0, /* 7  */
    0, /* 8  */
    1, /* 9  */
    1, /*10  */
    1, /*11  */
    0, /*12  */
    1, /*13  */
    1, /*14  */
    1, /*15  */
    1, /*16  */
    1, /*17  */
    0, /*18  */
    0, /*19  */
    0, /*20  */
    1, /*21  */
    0, /*22  */
    1, /*23  */
    1, /*24  */
    1, /*25  */
    0, /*26  */
    0, /*27  */
    0, /*28  */
    0, /*29  */
    0, /*30  */
    1, /*31  */
    0, /*32  */
    0, /*33  */
    0, /*34  */
    1, /*35  */
    1, /*36  */
    0, /*37  */
    1, /*38  */
    1, /*39  */
    0, /*40  */
    1, /*41  */
    0  /*42  */
};

/* Kronecker symbol (43 | p) for odd prime p.  For p = 2 it returns -1. */
static inline int kronecker_43_u64(uint64_t p)
{
    if (p == 2) return -1;        /* (43|2) = -1 */
    if (p == 43) return 0;        /* (43|43) = 0 */
    unsigned r = (unsigned)(p % 43ULL);
    int leg = residue[r] ? 1 : -1;            /* (p|43) */
    int sign = ((p % 4ULL) == 1ULL) ? 1 : -1; /* (-1)^((p-1)/2) */
    return leg * sign;                                 /* (43|p) */
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }

            if (p == 43)
            {
                /* contribution is 1, nothing to do */
            }
            else
            {
                int k = kronecker_43_u64(p);
                if (k == 1)
                    result *= (seq_t)(exp + 1);          /* a(p^e) = e+1 */
                else /* k == -1 */
                    if (exp & 1)                           /* odd exponent -> 0 */
                        return 0;
                    /* even exponent -> a(p^e) = 1, nothing to multiply */
            }
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1)
    {
        uint64_t p = temp;
        uint64_t exp = 1;

        if (p == 43)
        {
            /* contribution is 1, nothing to do */
        }
        else
        {
            int k = kronecker_43_u64(p);
            if (k == 1)
                result *= (seq_t)(exp + 1);
            else /* k == -1 */
                if (exp & 1)
                    return 0;
        }
    }

    return result;
}
