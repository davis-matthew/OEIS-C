/*
 * A029833  A discrete version of the Mangoldt function: if n is prime then
 *          round(log(n)) else 0.
 * https://oeis.org/A029833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(log(n)) if n is prime, otherwise 0
 * Range:     u8 n <= 9577, u16 n <= 9577, u32 n <= 9577, u64 n <= 9577,
 *            u128 n <= 9577, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* primes only */
    if (!is_prime(n) || n <= 1)
        return 0;

    /* ln(2) * 2^32  (integer scaled constant) */
    const unsigned __int128 LN2_2P32 = 0xB17217F7ULL;   /* ≈ 2^32 * ln(2) */

    /* integer exponent e = floor(log2(n)) */
    int e = 63 - __builtin_clzll((unsigned long long)n);

    /* n = (1 + frac/2^32) * 2^e   with 0 ≤ frac < 2^32 */
    unsigned __int128 shifted = (unsigned __int128)n << 32;
    unsigned __int128 val32   = shifted >> e;                 /* [2^32 , 2^33) */
    unsigned __int128 frac    = val32 - ((unsigned __int128)1 << 32); /* [0 , 2^32) */

    /* ln(n) * 2^32 = e * ln2 * 2^32 + (frac * ln2) */
    unsigned __int128 part1 = (unsigned __int128)e * LN2_2P32;
    unsigned __int128 part2 = (unsigned __int128)LN2_2P32 * frac >> 32;
    unsigned __int128 ln_scaled = part1 + part2;

    /* round to nearest integer: add 0.5*2^32 and shift right */
    unsigned __int128 rounded = (ln_scaled + ((unsigned __int128)1 << 31)) >> 32;

    return (seq_t)rounded;
}
