/*
 * A117981  A modified Legendre-binomial transform of 2^n for p=3.
 * https://oeis.org/A117981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A117981
    Formula: a(n) = Sum_{k=0..n} (-1)^(n-k) * L(C(n,k)/3) * 2^k
    where L(j/p) is the Legendre symbol of j and p=3.
    All other requirements are satisfied below.    */

#include <stdbool.h>
#include "../utils/mul_small.h"

/* Precomputed C(ni,ki) mod 3 for ni,ki ∈ {0,1,2}. */
static const unsigned char comb_mod3[3][3] = {
    {1, 0, 0},          /* C(0,0)=1 */
    {1, 1, 0},          /* C(1,0)=1, C(1,1)=1 */
    {1, 2, 1}           /* C(2,0)=1, C(2,1)=2, C(2,2)=1 */
};

/* Compute C(n,k) mod 3 using Lucas' theorem.  */
static unsigned binom_mod3(idx_t n, idx_t k)
{
    unsigned res = 1;
    while (n || k) {
        unsigned ni = n % 3;
        unsigned ki = k % 3;
        if (ki > ni)
            return 0;          /* C(ni,ki) = 0 */
        res = (res * comb_mod3[ni][ki]) % 3;
        n /= 3;
        k /= 3;
    }
    return res;                   /* C(0,0)=1 */
}

/* Main sequence function.   */
seq_t a(idx_t n)
{
    /* Fast paths for the very small indices are unnecessary; the
       general loop handles them correctly and keeps the code short. */
    seq_t sum_pos = 0,  /* sum of positive contributions      */
          sum_neg = 0;  /* sum of negative contributions      */

    seq_t power2 = 1;        /* 2^k, updated each iteration        */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned bm = binom_mod3(n, k);         /* C(n,k) mod 3          */
        if (bm != 0) {                          /* 3 ∤ C(n,k) → contribution */
            int legendre = (bm == 1) ? 1 : -1;   /* Legendre symbol of C(n,k)/3   */
            int parity   = ((n - k) & 1) ? -1 : 1; /* (-1)^(n-k)                     */
            int sign     = legendre * parity;   /* total sign                     */

            if (sign > 0)
                sum_pos += power2;
            else
                sum_neg += power2;
        }
        power2 = mul_small(power2, 2);           /* next power of two             */
    }

    /* The definition guarantees positivity: sum_pos ≥ sum_neg */
    return sum_pos - sum_neg;
}
