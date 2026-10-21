/*
 * A079866  a(1)=1 and for n>1: floor(n^(1/Omega(n))), where Omega(n) is the
 *          total number of prime factors of n (A001222).
 * https://oeis.org/A079866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(1/Omega(n))) (Omega(n) = total number of prime
 *            factors of n, counted with multiplicity)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/factor_u64.h"

/* Integer k‑th root: the greatest integer r such that r^k <= n. */
static seq_t int_root(seq_t n, idx_t k)
{
    if (k == 1)
        return n;              /* trivial case */
    seq_t low = 1, high = n;   /* n >= 1 */
    while (low < high) {
        seq_t mid = (low + high + 1) / 2;
        seq_t val = seq_pow(mid, k);
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;             /* Omega(1) is undefined; by definition a(1)=1 */

    uint64_t m = (uint64_t)n;
    /* use a small stack‑allocated buffer: max 64 prime factors for 64‑bit n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    idx_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (idx_t)exps[i];

    /* omega > 0 for n > 1 */
    return int_root((seq_t)n, omega);
}
