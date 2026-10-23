/*
 * A183092  a(n) is the product of divisors d of n such that d is not equal to
 *          m^k where m = noncomposite number, k >= 1.
 * https://oeis.org/A183092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all divisors d of n that are not pure prime
 *            powers /* (i.e. d ≠ p^k for any prime p and integer k ≥ 1)
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 119, u64 n <= 239,
 *            u128 n <= 839, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: recursive traversal of all divisors of n based on its prime factorisation.
   primes[0..cnt-1] and exps[0..cnt-1] hold the distinct prime factors and
   their exponents.  cur   holds the divisor built so far.
   used  counts how many distinct primes have been used in cur (0 for 1,
   1 for pure prime power, ≥2 for other divisors).  res  accumulates the desired
   product. */
static void rec_divisors(const uint64_t *primes, const int *exps, int cnt,
                         int depth, seq_t cur, int used, seq_t *res)
{
    if (depth == cnt) {
        /* Accept cur if it is not a pure prime power (used == 0 or used > 1). */
        if (used == 0 || used > 1)
            *res *= cur;
        return;
    }

    seq_t power = 1;
    for (int e = 0; e <= exps[depth]; ++e) {
        rec_divisors(primes, exps, cnt, depth + 1,
                     cur * power, used + (e > 0), res);
        power *= (seq_t)primes[depth];
    }
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by the OEIS offset. */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    int      exps[64];
    int      cnt = 0;

    /* Simple trial‑division factorisation (fast enough for n ≤ 10 000). */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                     /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = 1;
    rec_divisors(primes, exps, cnt, 0, (seq_t)1, 0, &result);
    return result;
}
