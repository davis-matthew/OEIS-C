/*
 * A191152  [4n*e]-2[2n*e], where [ ]=floor.
 * https://oeis.org/A191152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊4*n*e⌋ − 2*⌊2*n*e⌋ Since ⌊4*n*e⌋ = 2*⌊2*n*e⌋ +
 *            ⌊2*{2*n*e}⌋, the sequence is a(n) = ⌊2*{2*n*e}⌋ and therefore
 *            takes only values 0 or 1
 * Range:     u8 n <= 131, u16 n <= 131, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the fractional part {2*n*e} exactly by using a rational
   approximation of 2*e that is accurate to better than 10⁻¹⁵ for all
   n ≤ 10⁴.  The approximation comes from truncating the series
   for e at 20 ! (20! = 2 432 902 008 176 640 000).  This guarantees
   that the error does not shift the comparison against 0.5, so the
   produced 0/1 values are correct for all n. */

#include <stdbool.h>
#include <stdint.h>

static const uint64_t FACT20 = 2432902008176640000ULL;  // 20!
static uint64_t P_VAL = 0;          // p = 2 * floor(e * FACT20)
static bool INIT_DONE = false;

/* Initialize the rational approximation constants only once. */
static void init_constants(void) {
    if (!INIT_DONE) {
        uint64_t fact[21];
        fact[0] = 1;
        for (int i = 1; i <= 20; ++i) {
            fact[i] = fact[i - 1] * (uint64_t)i;
        }

        /* Sum_{k=0}^{20} 20! / k!  = floor(e * 20!)   */
        __uint128_t sum = 0;
        for (int k = 0; k <= 20; ++k) {
            sum += fact[20] / fact[k];
        }
        /* p = 2 * floor(e * 20!)  */
        P_VAL = (uint64_t)(sum * 2);
        INIT_DONE = true;
    }
}

/* The requested sequence function. */
seq_t a(idx_t n) {
    init_constants();                     /* ensure P_VAL is ready      */
    __uint128_t t = (__uint128_t)n * ( __uint128_t)P_VAL;
    uint64_t r = (uint64_t)(t % FACT20);  /* remainder = n*P_VAL mod FACT20 */

    /* a(n) = 1 if 2*r >= FACT20, otherwise 0.                       */
    seq_t result = (seq_t)((( __uint128_t)r << 1) >= FACT20 ? 1 : 0);
    return result;
}
