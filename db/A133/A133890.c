/*
 * A133890  Binomial(n+10,n) mod 10.
 * https://oeis.org/A133890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10) mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   crt_mod10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/crt_mod10.h"

static inline uint8_t binom_mod2(idx_t N, idx_t K)
{
    /* Lucas theorem base‑2: binom(N,K) ≡ 1 (mod 2) iff for every bit
       where K has 1, N also has 1.  Here K = 10 = (1010)_2.  */
    /* So we just check that bits 3 and 1 of N are set. */
    return (uint8_t)(((N & 10ULL) == 10ULL) ? 1U : 0U);
}

static inline uint8_t binom_mod5(idx_t N, idx_t K)
{
    /* K = 10 = (20)_5.  By Lucas theorem:
       binom(N,10) mod 5 = binom( (N/5) mod 5 , 2 ) mod 5,
       provided the lower digit of K (0) is ≤ lower digit of N.
       The lower digit condition is always true because K’s low digit is 0.
    */
    uint8_t n1 = (uint8_t)((N / 5) % 5);          /* higher digit of N in base 5 */
    if (n1 < 2)
        return 0;                                 /* binom(n1,2)=0 when n1<2 */

    /* binom(n1,2) = n1*(n1-1)/2 .  n1 is 2,3,4 so it fits in 8 bits. */
    uint8_t val = (uint8_t)(n1 * (n1 - 1) / 2);
    return val % 5;
}

seq_t a(idx_t n)
{
    idx_t N = n + 10ULL;                      /* N = n+10 */
    uint8_t mod2 = binom_mod2(N, 10ULL);
    uint8_t mod5 = binom_mod5(N, 10ULL);
    uint8_t res  = crt_mod10(mod2, mod5);
    return (seq_t)res;
}
