/*
 * A232267  Least k>=0 such that n^3 + k^2 is a square.
 * https://oeis.org/A232267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 0 such that n^3 + k^2 is a square
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_PRIME 10000

/* simple sieve for prime generation */
static unsigned char isprime[MAX_PRIME + 1];
static unsigned int primes[1220];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_sieve(void)
{
    if (primes_inited) return;
    for (int i = 2; i <= MAX_PRIME; ++i) isprime[i] = 1;
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (isprime[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i) isprime[j] = 0;
        }
    }
    for (int i = 2; i <= MAX_PRIME; ++i) if (isprime[i])
        primes[primes_cnt++] = (unsigned int)i;
    primes_inited = true;
}

/* Recursive generation of all divisors of n^3. */
static void gen_divs(const unsigned int *fprimes, const unsigned int *fexps,
                     size_t fcnt, idx_t cur, size_t idx, idx_t *out,
                     size_t *outcnt)
{
    if (idx == fcnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    unsigned int p = fprimes[idx];
    unsigned int exp = fexps[idx];
    idx_t pw = 1;
    for (unsigned int e = 0; e <= exp; ++e) {
        gen_divs(fprimes, fexps, fcnt, cur * pw, idx + 1, out, outcnt);
        pw *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* a(0) = 0 */

    init_sieve();

    /* factor n and build exponents for n^3 */
    idx_t temp = n;
    unsigned int fprimes[12];
    unsigned int fexps[12];
    size_t fcnt = 0;

    for (size_t i = 0; i < primes_cnt &&
         (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        unsigned int p = primes[i];
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fprimes[fcnt] = p;
            fexps[fcnt]   = e * 3;   /* exponent in n^3 */
            ++fcnt;
        }
    }
    if (temp > 1) {  /* remaining prime factor */
        fprimes[fcnt] = (unsigned int)temp;
        fexps[fcnt]   = 3;  /* exponent 1 * 3 */
        ++fcnt;
    }

    /* compute n^3 as idx_t */
    idx_t n3 = (idx_t)n * (idx_t)n * (idx_t)n;

    /* generate all divisors of n^3 */
    idx_t divs[2000];
    size_t dcnt = 0;
    gen_divs(fprimes, fexps, fcnt, 1, 0, divs, &dcnt);

    /* find minimal k */
    idx_t min_k = n3;  /* upper bound */

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t a_val = divs[i];
        idx_t b_val = n3 / a_val;
        if (a_val > b_val) continue;                      /* keep a <= b */
        if ((a_val & (idx_t)1) != (b_val & (idx_t)1)) continue;  /* parity check */
        idx_t k = (b_val - a_val) >> 1;
        if (k < min_k) min_k = k;
    }

    return (seq_t)min_k;
}
