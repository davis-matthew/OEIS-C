/*
 * A143959  Final digit of n^(n+1)-(n+1)^n for n>2.
 * https://oeis.org/A143959
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = last digit of n^(n+1)-(n+1)^n
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided helper: 
   seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod);
*/

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined, but we can still compute it. */
    seq_t mod = 10;
    seq_t term1 = mod_pow((seq_t)n, n + 1, mod);
    seq_t term2 = mod_pow((seq_t)(n + 1), n, mod);
    seq_t result = (term1 + mod - term2) % mod;
    return result;
}
