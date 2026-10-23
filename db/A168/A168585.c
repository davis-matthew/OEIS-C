/*
 * A168585  Number of ways of partitioning the multiset {1,1,2,3,...,n-1} into
 *          exactly five nonempty parts.
 * https://oeis.org/A168585
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*5^(n-2) - 10*4^(n-2) + 14*3^(n-2) - 3*2^n + 7) / 24
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A168585 for any n >= 5. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 5; for smaller n the value
       is undefined in the OEIS entry. We return 0 for n < 5. */
    if (n < 5)
        return (seq_t)0;

    seq_t nseq = (seq_t)n;            /* cast index to seq_t */
    seq_t pow2  = big_pow((seq_t)2, nseq);           /* 2^n                      */
    seq_t pow3  = big_pow((seq_t)3, nseq - (seq_t)2); /* 3^(n-2)                  */
    seq_t pow4  = big_pow((seq_t)4, nseq - (seq_t)2); /* 4^(n-2)                  */
    seq_t pow5  = big_pow((seq_t)5, nseq - (seq_t)2); /* 5^(n-2)                  */

    seq_t term1 = (seq_t)3 * pow5;
    seq_t term2 = (seq_t)10 * pow4;
    seq_t term3 = (seq_t)14 * pow3;
    seq_t term4 = (seq_t)3 * pow2;

    seq_t numer = term1 - term2 + term3 - term4 + (seq_t)7;
    seq_t result = numer / (seq_t)24;   /* division is exact for all valid n */

    return result;
}
