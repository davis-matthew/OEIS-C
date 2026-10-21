/*
 * A088121  Smallest prime obtained as a sum of n terms of a geometric
 *          progression + the common ratio, or 0 if no such terms exists.
 *          Smallest prime of the form (a +ar +ar^2 + ar^3 +... ) + r.
 * https://oeis.org/A088121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime of the form a*(2^n-1)+2 with a>=1. If no
 *            such prime exists, a(n)=0
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    seq_t base = big_pow((seq_t)2, (seq_t)n); /* base = 2^n  */
    base = base - (seq_t)1;        /* base = 2^n - 1 */

    /* try successive values of a (starting at 1) until a prime is found */
    uint64_t a_val = 1;
    while (1) {
        seq_t cand = base * ((seq_t)a_val) + (seq_t)2; /* a*base + 2 */
        if (is_prime_big(cand)) {
            return cand;   /* smallest prime found */
        }
        a_val++;
    }
    /* unreachable, but keep the compiler happy */
    return (seq_t)0;
}
