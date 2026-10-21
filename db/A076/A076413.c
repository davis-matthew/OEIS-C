/*
 * A076413  Number of ways n is expressible as the least common multiple of a
 *          nonempty set of positive integers not containing either 1 or n.
 * https://oeis.org/A076413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 239, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{J⊆P} (-1)^{|J|} * 2^{(∏_{p∈J} e_p)*(∏_{p∉J} (e_p+1)) - (J==∅?2:1)}
 * where P is the set of distinct prime divisors of n and e_p is their exponents.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* a(1) = 0 */

    /* Factor n into prime powers */
    idx_t primes[64];
    idx_t exps[64];
    idx_t k = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[k] = p;
            exps[k]   = e;
            ++k;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        primes[k] = tmp;
        exps[k]   = 1;
        ++k;
    }

    /* Inclusion–exclusion over subsets of the k primes */
    seq_t result = 0;
    idx_t total_masks = ((idx_t)1) << k;
    for (idx_t mask = 0; mask < total_masks; ++mask) {
        /* product of e_p or (e_p+1) depending on whether p is in the subset */
        idx_t prod = 1;
        for (idx_t i = 0; i < k; ++i) {
            prod *= (mask & ((idx_t)1 << i)) ? exps[i] : (exps[i] + 1);
        }
        idx_t sub  = (mask == 0) ? 2 : 1;
        idx_t s    = prod - sub;           /* s ≥ 0 for every mask */
        seq_t term = pow2((idx_t)s);

        if ((__builtin_popcountll((unsigned long long)mask) & 1ULL) == 0)
            result += term;                /* add for even |mask| */
        else
            result -= term;                /* subtract for odd |mask| */
    }
    return result;
}
