/*
 * A153193  a(n) is the number of integers of the form n*(n+1)*k / (k -
 *          n*(n+1)) where k is an integer >= 1.
 * https://oeis.org/A153193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k ≥ 1 for which n*(n+1)*k/(k –
 *            n*(n+1)) is an integer
 * Range:     u8 n <= 34, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_divs_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_divs_rec.h"

/* ---------- prime sieve ---------- */
static int primes_done = 0;
#define PRIME_LIMIT 100000
static uint32_t primes[9592];   /* number of primes ≤ 100000 */
static size_t prime_cnt = 0;

static void init_primes(void)
{
    char is_comp[PRIME_LIMIT + 1] = {0};
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_done = 1;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!primes_done)
        init_primes();

    if (n == 0)          /* offset is 1; result for n=0 is unused */
        return (seq_t)0;

    uint64_t D = (uint64_t)n * ((uint64_t)n + 1ULL);

    /* factorisation of D */
    uint32_t pf[20];
    int     exp[20];
    int pf_cnt = 0;
    uint64_t tmp = D;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[pf_cnt] = p;
            exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        pf[pf_cnt] = (uint32_t)tmp;
        exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* tau(D^2) = product (2*e_i + 1) */
    unsigned long long tau = 1ULL;
    for (int i = 0; i < pf_cnt; ++i)
        tau *= (unsigned long long)(2ULL * exp[i] + 1ULL);

    /* count divisors of D^2 that are ≤ D-1 */
    int exp2[20];
    for (int i = 0; i < pf_cnt; ++i)
        exp2[i] = 2 * exp[i];

    unsigned long long f = 0ULL;
    count_divs_rec(pf, exp2, 0, pf_cnt, 1ULL, D - 1ULL, &f);

    unsigned long long result = tau + f;
    return (seq_t)result;
}
