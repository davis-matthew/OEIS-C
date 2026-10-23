/*
 * A130840  a(n) = floor((1/16)*(16 + 2^n - 8*n + 8*n^2)).
 * https://oeis.org/A130840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((16 + 2^(n-1) - 8*(n-1) + 8*(n-1)^2)/16) (offset 1)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return 2^exp as a seq_t.
   Uses exponentiation by squaring; safe for exp <=63 when seq_t is 64‑bit. */
static seq_t pow2_exp(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    idx_t k = n - 1;               /* work with k = n‑1 */

    if (n < 5) {                  /* direct evaluation for small n */
        seq_t powk = pow2_exp(k); /* 2^(n-1), here k<=3 */
        seq_t numer = (seq_t)16 + powk - (seq_t)8 * k + (seq_t)8 * k * k;
        return numer / (seq_t)16;
    }

    /* For n ≥ 5 use the simplified form
         a(n) = 2^(n-5) + ((k^2 - k + 2) >> 1)   with k = n-1
    */
    seq_t term1 = pow2_exp(n - 5);            /* 2^(n-5)   */
    seq_t term2 = ((seq_t)k * k - k + 2) >> 1;/* (k^2 - k + 2) / 2  */
    return term1 + term2;
}
