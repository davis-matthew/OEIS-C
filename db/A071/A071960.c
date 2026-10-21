/*
 * A071960  Largest k >= 0 such that Product_{i=0..k} (n+i) divides n!.
 * https://oeis.org/A071960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=largest k≥0 such that product_{i=0..k}(n+i) divides n!
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   legendre_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/legendre_factorial.h"

/* maximum value needed for sieve – enough for all test cases (n≤10000) */
#define MAX_LIMIT 200000
#define MAX_PRIMES 20000

static int spf[MAX_LIMIT + 1];
static int prime_idx_map[MAX_LIMIT + 1];
static int prime_list[MAX_PRIMES];
static int prime_count = 0;

/* helper arrays reused for every call */
static int fact_exp[MAX_PRIMES];
static int cur_exp[MAX_PRIMES];

/* one‑time sieve initialization */
static void init_sieve(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    for (int i = 0; i <= MAX_LIMIT; ++i) {
        spf[i] = 0;
        prime_idx_map[i] = -1;
    }

    for (int i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_idx_map[i] = prime_count;
            prime_list[prime_count++] = i;
            if ((long long)i * i <= MAX_LIMIT) {
                for (int j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* main OEIS function */
seq_t a(idx_t n)
{
    init_sieve();

    if (n == 0) return 0;          /* undefined for offset <1, but guard */
    unsigned int un = (unsigned int)n;

    /* factorial exponents */
    for (int i = 0; i < prime_count; ++i) fact_exp[i] = 0;
    for (int i = 0; i < prime_count; ++i) {
        int p = prime_list[i];
        if (p > un) break;
        fact_exp[i] = legendre_factorial(un, p);
    }

    /* current product exponents – start with n */
    for (int i = 0; i < prime_count; ++i) cur_exp[i] = 0;
    unsigned int t = un;
    while (t > 1) {
        int sp = spf[t];
        int idx = prime_idx_map[sp];
        int cnt = 0;
        while (t % sp == 0) { t /= sp; cnt++; }
        cur_exp[idx] += cnt;
    }

    int result = 0; /* current k (product up to n+result) */
    while (1) {
        ++result;                         /* try k = result */
        unsigned int x = un + result;     /* new term n+result */

        /* factor the new term and update cur_exp; stop immediately if overflow */
        unsigned int temp = x;
        while (temp > 1) {
            int sp;
            if (temp <= MAX_LIMIT)
                sp = spf[temp];
            else {
                /* fallback trial division (should not occur for given limits) */
                sp = 0;
                for (int i = 0; i < prime_count && (long long)prime_list[i] * prime_list[i] <= temp; ++i) {
                    int p = prime_list[i];
                    if (temp % p == 0) { sp = p; break; }
                }
                if (sp == 0) sp = temp;    /* temp is prime */
            }

            int idx = prime_idx_map[sp];
            if (idx < 0) return result - 1;  /* prime larger than sieve => impossible */

            int cnt = 0;
            while (temp % sp == 0) { temp /= sp; cnt++; }

            cur_exp[idx] += cnt;
            if (cur_exp[idx] > fact_exp[idx]) return result - 1; /* failure */
        }
    }
}
