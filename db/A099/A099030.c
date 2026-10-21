/*
 * A099030  Number of tone-rows in n-tone music.
 * https://oeis.org/A099030
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ((n-1)! + (n-1)!!)/4           if n is odd
 *              ((n-1)! + (n/2+1)(n-2)!!)/4       if n is even
 * (n, a(n)) starts at n = 3.  The following implementation computes
 * the two terms separately, each guaranteed to be divisible by 4 for n ≥ 5,
 * which avoids overflow for 64‑bit builds while remaining exact for
 * arbitrary‑precision builds.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(3) and a(4) are the only cases where the split into two
     * divisible-by‑4 terms fails.  They are handled explicitly.  */
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)3;

    /* ------------------------------------------------------------------
     * 1. Compute (n-1)! ÷ 4 without overflow.
     *    We divide out two factors of 2 as soon as possible.
     * ------------------------------------------------------------------ */
    seq_t fact_over4 = (seq_t)1;
    uint64_t twos_to_divide = 2;  /* we need to divide by 4 = 2² */
    for (idx_t i = 2; i <= n - 1; ++i) {
        seq_t val = (seq_t)i;
        /* strip factors of 2 until we have removed two of them */
        while (twos_to_divide && (val & 1) == 0) {
            val >>= 1;
            twos_to_divide--;
        }
        fact_over4 *= val;
    }

    /* ------------------------------------------------------------------
     * 2. Compute the second term of the formula.
     *    For n ≥ 5 both sub‑terms are divisible by 4, allowing us to
     *    shift right by two bits after forming the product.
     * ------------------------------------------------------------------ */
    seq_t term2;
    if (n & 1) {  /* n odd : (n-1)!! belongs to even numbers 2..n-1 */
        seq_t dbl = (seq_t)1;
        for (idx_t j = 2; j <= n - 1; j += 2) {
            dbl *= (seq_t)j;
        }
        term2 = dbl >> 2;      /* division by 4 */
    } else {     /* n even : ((n/2)+1)(n-2)!!  */
        seq_t dbl = (seq_t)1;
        for (idx_t j = 2; j <= n - 2; j += 2) {
            dbl *= (seq_t)j;
        }
        dbl *= (seq_t)(n / 2 + 1);
        term2 = dbl >> 2;      /* division by 4 */
    }

    return fact_over4 + term2;
}
