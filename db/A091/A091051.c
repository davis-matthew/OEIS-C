/*
 * A091051  Sum of divisors of n that are perfect powers.
 * https://oeis.org/A091051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all divisors d of n such that d is a perfect power
 *            i^j, i ≥ 1, j ≥ 2. The sum uses each divisor exactly once (e.g.
 *            64 contributes 64 once, even though 64 = 2^6 = 4^3 = 8^2)
 * Range:     u8 n <= 32, u16 n <= 8940, u32 n <= 8940, u64 n <= 8940,
 *            u128 n <= 8940, big any n
 * Helpers:   big_pow, bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/bitlen_seq_t.h"

/* Return true if x is a perfect power (x = i^j, j ≥ 2).  The test uses
   generic arithmetic on seq_t, no overflow assumptions. */
static bool is_perfect_power(seq_t x)
{
    /* 1 = 1^j for any j ≥ 2 */
    if (x == (seq_t)1) return true;

    /* Maximum exponent is the bit length of x minus 1
       (since 2^(bitlen-1) ≤ x < 2^bitlen). */
    unsigned int max_exp = bitlen_seq_t(x);
    for (unsigned int exp = 2; exp <= max_exp; ++exp) {
        /* Binary search for integer r such that r^exp = x. */
        seq_t low = (seq_t)2;
        seq_t high = x;
        while (low <= high) {
            seq_t mid = (low + high) >> 1;
            seq_t p = big_pow(mid, (seq_t)exp);
            if (p == x) return true;
            if (p < x) low = mid + (seq_t)1;
            else high = mid - (seq_t)1;
        }
    }
    return false;
}

/* Enumerate all divisors of n and return the sum of those that are
   perfect powers.  The function works for any build where seq_t is
   an integral type or an arbitrary‑precision class. */
seq_t a(idx_t n)
{
    /* Factor n by trial division.  n fits in 64‑bit, so a simple
       trial division loop is sufficiently fast for n ≤ 10 000. */
    idx_t nn = n;
    idx_t primes[64];
    size_t exps[64];
    size_t pc = 0;

    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= nn; ++p) {
        if (nn % p == 0) {
            size_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (nn > 1) {           /* remaining prime factor */
        primes[pc]   = nn;
        exps[pc]     = 1;
        ++pc;
    }

    /* Compute total number of divisors: product of (exponent+1). */
    size_t tot = 1;
    for (size_t i = 0; i < pc; ++i) tot *= (exps[i] + 1);

    /* Allocate array for divisors.  Cast void* to seq_t* for C++. */
    seq_t *divs = (seq_t *)malloc(tot * sizeof(seq_t));
    if (!divs) return (seq_t)0;   /* allocation failure → 0 */

    size_t div_cnt = 1;
    divs[0] = (seq_t)1;

    /* Generate all divisors by multiplying existing ones by powers of
       each prime factor. */
    for (size_t i = 0; i < pc; ++i) {
        seq_t prime = (seq_t)primes[i];
        size_t mult  = exps[i];
        size_t cur_cnt = div_cnt;
        seq_t prime_pow = (seq_t)1;
        for (size_t e = 1; e <= mult; ++e) {
            prime_pow *= prime;
            for (size_t j = 0; j < cur_cnt; ++j) {
                divs[div_cnt++] = divs[j] * prime_pow;
            }
        }
    }

    /* Sum the divisors that are perfect powers. */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < div_cnt; ++i) {
        if (is_perfect_power(divs[i]))
            sum += divs[i];
    }

    free(divs);
    return sum;
}
