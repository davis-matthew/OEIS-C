/*
 * A130856  The digital root (A010888) of the Catalan numbers A000108.
 * https://oeis.org/A130856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the digital root of the Catalan number C(n) =
 *            binom(2n,n)/(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digital root is congruent to the number modulo 9 (0 is replaced by 9). */

#include <stdint.h>
#include "../utils/exp_power.h"

/* Fixed limit sufficient for n up to 10 000 (2n ≤ 20 000). 2262 is the number of primes ≤20000. */
#define MAX_PRIME 20000
static int primes[2262];
static int prime_cnt = 0;

/* One‑time sieve to fill primes[ ] up to MAX_PRIME. */
static void init_primes(void)
{
    static int done = 0;
    if (done) return;
    static unsigned char composite[MAX_PRIME + 1];
    for (int p = 2; p <= MAX_PRIME; ++p) {
        if (!composite[p]) {
            primes[prime_cnt++] = p;
            if ((long long)p * p <= MAX_PRIME) {
                for (long long q = (long long)p * p; q <= MAX_PRIME; q += p)
                    composite[(int)q] = 1;
            }
        }
    }
    done = 1;
}

/* Exponent of prime p in n!  (Legendre's formula). */
static uint64_t exp_factorial(uint64_t n, uint64_t p)
{
    uint64_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Compute base^exp modulo 9 using fast exponentiation. */
static unsigned pow_mod9(unsigned base, uint64_t exp)
{
    unsigned result = 1 % 9;
    unsigned b = base % 9;
    while (exp) {
        if (exp & 1ULL)
            result = (result * b) % 9;
        b = (b * b) % 9;
        exp >>= 1ULL;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    uint64_t two_n = (uint64_t)2 * n;
    unsigned rem = 1;               /* product modulo 9 */

    for (int idx = 0; idx < prime_cnt && (uint64_t)primes[idx] <= two_n; ++idx) {
        uint64_t p = (uint64_t)primes[idx];

        uint64_t e2n   = exp_factorial(two_n,   p);
        uint64_t en    = exp_factorial((uint64_t)n, p);
        uint64_t en1   = exp_power((uint64_t)n + 1, p);

        int64_t e = (int64_t)e2n - 2 * (int64_t)en - (int64_t)en1;
        if (e <= 0) continue;        /* no factor of p in the Catalan number */

        if (p == 3) {
            /* 3^2 ≡ 0 (mod 9). Any exponent ≥ 2 makes the product 0. */
            if (e >= 2) {
                rem = 0;
                break;
            }
            rem = (rem * 3) % 9;      /* e == 1 */
        } else {
            unsigned pow_res = pow_mod9((unsigned)(p % 9), e);
            rem = (rem * pow_res) % 9;
            if (rem == 0) break;
        }
    }

    unsigned result = (rem == 0) ? 9 : rem;
    return (seq_t)result;
}
