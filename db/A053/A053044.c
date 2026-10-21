/*
 * A053044  a(n) is the number of iterations of the Euler totient function to
 *          reach 1, starting at n!.
 * https://oeis.org/A053044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003434(A000142(n))
 * Range:     u8 n <= 65, u16 n <= 6901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_N        10000
#define MAX_PRIMES   2000
#define MAX_FACTORS  10

typedef struct { int prime_idx; int exp; } Factor;

static bool initialized = false;
static int primes[MAX_PRIMES];
static int prime_count;
static int spf[MAX_N + 1];
static int prime_index_of[MAX_N + 1];
static Factor factor_list[MAX_N + 1][MAX_FACTORS];
static int factor_len[MAX_N + 1];

static void init(void)
{
    int i, j;
    /* smallest prime factor for every number */
    for (i = 0; i <= MAX_N; ++i) spf[i] = i;
    for (i = 2; i * i <= MAX_N; ++i) {
        if (spf[i] == i) {                    /* i is prime */
            for (j = i * i; j <= MAX_N; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* list of primes and index lookup */
    prime_count = 0;
    for (i = 0; i <= MAX_N; ++i) prime_index_of[i] = -1;
    for (i = 2; i <= MAX_N; ++i) {
        if (spf[i] == i) {
            primes[prime_count] = i;
            prime_index_of[i] = prime_count;
            ++prime_count;
        }
    }

    /* factorisation of every number up to MAX_N */
    for (i = 0; i <= MAX_N; ++i) factor_len[i] = 0;
    factor_len[1] = 0;
    for (i = 2; i <= MAX_N; ++i) {
        int x = i;
        while (x > 1) {
            int p = spf[x];
            int e = 0;
            while (x % p == 0) { x /= p; ++e; }
            int idx = prime_index_of[p];
            int fj = factor_len[i];
            factor_list[i][fj].prime_idx = idx;
            factor_list[i][fj].exp       = e;
            ++factor_len[i];
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n <= 1) return (seq_t)0;

    static int cur[MAX_PRIMES];
    static int nxt[MAX_PRIMES];
    int i;

    /* clear current exponents */
    for (i = 0; i < prime_count; ++i) cur[i] = 0;

    /* factorial prime exponents */
    for (i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if (p > (int)n) break;
        int e = 0;
        int power = p;
        while (power <= (int)n) {
            e += (int)(n / power);
            if (power > (int)(n / p)) break;
            power *= p;
        }
        cur[i] = e;
    }

    int steps = 0;
    for (;;) {
        bool all_zero = true;
        for (i = 0; i < prime_count; ++i)
            if (cur[i]) { all_zero = false; break; }
        if (all_zero) break;

        for (i = 0; i < prime_count; ++i) nxt[i] = 0;

        for (i = 0; i < prime_count; ++i) {
            int e = cur[i];
            if (!e) continue;
            if (e - 1 > 0) nxt[i] += e - 1;            /* p^(e-1)   */

            int p = primes[i];
            int val = p - 1;                          /* factors of p-1 */
            int fl = factor_len[val];
            for (int f = 0; f < fl; ++f) {
                int idx = factor_list[val][f].prime_idx;
                int expv = factor_list[val][f].exp;
                nxt[idx] += expv;
            }
        }

        for (i = 0; i < prime_count; ++i) cur[i] = nxt[i];
        ++steps;
    }

    return (seq_t)steps;
}
