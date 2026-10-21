/*
 * A093616  a(n) is the smallest k such that k*n has exactly as many divisors
 *          as n^2.
 * https://oeis.org/A093616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that the number of divisors of k*n equals
 *            * the number of divisors of n^2. * We factor n, then search all
 *            permutations of its exponent multiset * that satisfy 2*e_{σ(i)}
 *            ≥ e_i. The minimal k over these permutations * is returned
 * Range:     u8 n <= 67, u16 n <= 6883, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   permute_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/permute_rec.h"

/* smallest primes up to 100 for trial division */
static const uint32_t small_primes[25] = { 2,3,5,7,11,13,17,19,23,29,31,
                                           37,41,43,47,53,59,61,67,71,73,
                                           79,83,89,97 };

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n must be >= 1 in the OEIS sense */
    if (n == 0) return 0;   /* undefined, but safe */

    /* factor n */
    idx_t nn = n;
    idx_t primes[10];
    idx_t exps[10];
    size_t m = 0;

    for (size_t i = 0; i < 25 && (uint64_t)small_primes[i]*small_primes[i] <= nn; ++i) {
        uint32_t p = small_primes[i];
        if (nn % p == 0) {
            primes[m] = p;
            idx_t e = 0;
            while (nn % p == 0) { nn /= p; ++e; }
            exps[m] = e;
            ++m;
        }
    }
    if (nn > 1) {            /* remaining prime factor */
        primes[m] = nn;
        exps[m]   = 1;
        ++m;
    }

    if (m == 0) return (seq_t)1;    /* n == 1 */

    bool used[10] = { false };
    int  perm[10];
    seq_t best = 0;

    permute_rec(0, m, perm, used, primes, exps, &best);

    return best;
}
