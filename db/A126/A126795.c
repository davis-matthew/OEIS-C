/*
 * A126795  a(n) = gcd(n, Product_{p|n} (p+1)), where the product is over the
 *          distinct primes p that divide n.
 * https://oeis.org/A126795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( n, product_{p|n} (p+1) )
 * Range:     u8 n <= 3244, u16 n <= 9530, u32 n <= 9530, u64 n <= 9530,
 *            u128 n <= 9530, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;          /* unused; sequence starts at offset 1 */
    }

    /* factor_u64 returns the distinct prime factors of n in pr[0..cnt-1] */
    uint64_t pr[8];
    uint64_t ex[8];   /* unused, but required by factor_u64 signature */
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* product of (p+1) over distinct primes p|n, taken modulo n */
    uint64_t prod_mod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod_mod = (prod_mod * (pr[i] + 1)) % n;
    }

    /* result fits in seq_t because it is a divisor of n */
    seq_t n_seq = (seq_t)n;
    seq_t prod_seq = (seq_t)prod_mod;
    return gcd(prod_seq, n_seq);
}
