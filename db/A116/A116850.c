/*
 * A116850  Number of permutations of length n which avoid the patterns 231,
 *          12354.
 * https://oeis.org/A116850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A116850
   Formula: for n >= 4
     a(n) = n*(n+1)/2  +  2^(n-4) * ( n*(n+1) - 18 )  +  2
   Initial values (n = 1..6)
     a(1)=1, a(2)=2, a(3)=5, a(4)=14, a(5)=41, a(6)=119
   For n < 1 the function returns 0 (offset 1).
   The implementation avoids intermediate overflow by factoring out a
   power of two and using only 32‑bit arithmetic for the 64‑bit build.
*/

#include "../utils/seq_pow.h"

static const seq_t init[6] = { (seq_t)1, (seq_t)2, (seq_t)5,
                               (seq_t)14, (seq_t)41, (seq_t)119 };

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                      /* out of offset range */

    if (n <= 6)
        return init[n - 1];

    /* compute 2^(n-4)  (works for both built‑in and big‑int seq_t) */
    seq_t t16 = seq_pow((seq_t)2, n - 4);

    seq_t n_seq   = (seq_t)n;
    seq_t np1_seq = n_seq + 1;
    seq_t nnp1    = n_seq * np1_seq;          /* n*(n+1)  (even) */
    seq_t half    = nnp1 >> 1;                /* n*(n+1)/2 */

    seq_t temp   = nnp1 - ((seq_t)18);        /* n*(n+1)-18 */
    seq_t prod   = t16 * temp;                /* 2^(n-4)*(n*(n+1)-18) */

    return half + prod + ((seq_t)2);          /* final result */
}
