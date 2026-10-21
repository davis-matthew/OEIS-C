/*
 * A093653  Total number of 1's in binary expansion of all divisors of n.
 * https://oeis.org/A093653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{d|n} popcount(d) — total number of 1's in the binary
 *            expansion of all divisors of n
 * Range:     u8 n <= 9589, u16 n <= 9589, u32 n <= 9589, u64 n <= 9589,
 *            u128 n <= 9589, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper to generate all divisors of n' given its prime factorization.
   cnt   – number of distinct primes,
   idx   – current prime index,
   current – product of primes chosen so far,
   pr, ex – arrays of primes and their exponents,
   sum   – accumulated popcount sum (passed by pointer). */
static void gen_divisors(int idx, uint64_t current,
                         const uint64_t *pr, const int *ex,
                         int cnt, uint64_t *sum)
{
    if (idx == cnt) {
        *sum += __builtin_popcountll(current);
        return;
    }
    uint64_t p = pr[idx];
    int emax = ex[idx];
    uint64_t val = 1;
    int e;
    for (e = 0; e <= emax; ++e) {
        gen_divisors(idx + 1, current * val, pr, ex, cnt, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the problem specification. */
    uint64_t nn = (uint64_t)n;

    /* Factor n into primes. */
    uint64_t primes[20];
    int exps[20];
    int cnt = 0;
    uint64_t tmp = nn;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            do { tmp /= p; ++e; } while (tmp % p == 0);
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }

    uint64_t sum = 0;
    gen_divisors(0, 1, primes, exps, cnt, &sum);

    return (seq_t)sum;
}
