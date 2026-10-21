/*
 * A093360  a(n) = prime(n)^(n-1).
 * https://oeis.org/A093360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute p = prime(n) and exp = n-1, then p^exp */
    seq_t p   = (seq_t) get_prime(n);
    seq_t exp = (seq_t)(n - 1);
    return big_pow(p, exp);
}
