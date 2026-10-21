/*
 * A065103  A level 11 weight 5 form.
 * https://oeis.org/A065103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d | n} Kronecker(d/11)*(n/d)^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Kronecker symbol (d/11) for positive d.
   0 : d divisible by 11
   1 : quadratic residues modulo 11 (1,3,4,5,9)
  -1 : non‑residues modulo 11 (2,6,7,8,10) */
static const int residue[11] = {
    0,      /* 0  -> divisible by 11 */
    1,      /* 1  -> 1^2 */
   -1,      /* 2  -> not a residue */
    1,      /* 3  -> 5^2 */
    1,      /* 4  -> 2^2 */
    1,      /* 5  -> 4^2 */
   -1,      /* 6  -> not a residue */
   -1,      /* 7  -> not a residue */
   -1,      /* 8  -> not a residue */
    1,      /* 9  -> 3^2 */
   -1       /*10  -> not a residue */
};

static inline int kronecker11(idx_t d)
{
    return residue[d % 11];
}

/* Compute a(n) as defined above.  Works for any seq_t type
   that can hold the resulting integer.  The function is
   multiplicative and uses only simple integer arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0 */

    /* factor out any power of 11 */
    idx_t t11 = 0;
    idx_t temp = n;
    while (temp % 11 == 0) {
        temp /= 11;
        ++t11;
    }
    idx_t m = temp;   /* m is now coprime to 11 */

    /* compute 11^(4*t11) as unsigned __int128 (small enough). */
    unsigned __int128 pow11 = 1;
    for (idx_t i = 0; i < 4 * t11; ++i) pow11 *= 11;

    /* sum over divisors of m (which are all not divisible by 11) */
    long long sum = 0;          /* signed sum: ± values */
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i != 0) continue;

        /* divisor d1 = i */
        idx_t d1 = i;
        idx_t md1 = m / d1;
        unsigned long long term1 = md1 * (unsigned long long)md1 *
                                   (unsigned long long)md1 *
                                   (unsigned long long)md1;
        sum += (long long)kronecker11(d1) * (long long)term1;

        /* complementary divisor d2 = m / i */
        idx_t d2 = m / i;
        if (d1 != d2) {
            idx_t md2 = m / d2;
            unsigned long long term2 = md2 * (unsigned long long)md2 *
                                       (unsigned long long)md2 *
                                       (unsigned long long)md2;
            sum += (long long)kronecker11(d2) * (long long)term2;
        }
    }

    /* final result: sum * 11^(4*t11) */
    unsigned __int128 total = (unsigned __int128)(sum) * pow11;
    return (seq_t)total;
}
