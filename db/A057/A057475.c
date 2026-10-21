/*
 * A057475  Number of k, 1 <= k <= n, such that gcd(n,k) = gcd(n+1,k) = 1.
 * https://oeis.org/A057475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | 1 ≤ k ≤ n , gcd(k, n(n+1)) = 1 }
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_distinct, inclusion_exclusion, rec_inclusion
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_distinct.h"
#include "../utils/inclusion_exclusion.h"
#include "../utils/rec_inclusion.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    uint64_t primes[32];
    size_t cnt = 0;
    factor_distinct((uint64_t)n, primes, &cnt);

    uint64_t tmppr[32];
    size_t cnt2 = 0;
    uint64_t temp_n1 = (uint64_t)(n + 1);
    factor_distinct(temp_n1, tmppr, &cnt2);

    for (size_t i = 0; i < cnt2; ++i) primes[cnt++] = tmppr[i];

    return inclusion_exclusion((uint64_t)n, primes, cnt);
}
