/*
 * A083273  a(n) is the quotient of lcm of first n consecutive prime
 *          differences and A001223(n), the n-th difference between
 *          consecutive primes.
 * https://oeis.org/A083273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A080374(n) / A001223(n) where A080374(n) is the least
 *            common multiple of the first n prime gaps and A001223(n) is the
 *            nth prime gap (difference between the (n+1)th and nth primes)
 * Range:     u8 n <= 34, u16 n <= 217, u32 n <= 3644, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, pow_int_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*----------- Utilities ----------------------------------------------------*/
#include "../utils/is_prime.h"          /* for is_prime() if needed */
#include "../utils/pow_int_seq.h"       /* seq_t pow_int_seq(seq_t base, idx_t exp); */

/*----------- Prime sieve (once) ------------------------------------------*/
#define PRIME_LIMIT 200000                 /* enough for n ≈ 10000                */
#define MAX_PRIMES   20000                 /* number of primes below PRIME_LIMIT  */

static unsigned char comp[PRIME_LIMIT + 1] = {0};
static uint32_t primes[MAX_PRIMES];
static int primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;

    int cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            primes[cnt++] = (uint32_t)i;
            if (1LL * i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i) comp[j] = 1;
            }
        }
    }
    primes_cnt = cnt;
    primes_inited = true;
}

/* index of a prime within the primes array (returns -1 if not found) */
static int prime_index_of(uint32_t val)
{
    int lo = 0, hi = primes_cnt - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (primes[mid] == val) return mid;
        if (primes[mid] < val) lo = mid + 1; else hi = mid - 1;
    }
    return -1;
}

/*----------- Main OEIS sequence function --------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; value for n=0 is irrelevant */

    init_primes();             /* ensure primes array is populated */

    /* we need n+1 primes; LIMIT already large enough */

    /* exponent array for LCM of gaps seen so far, one per prime */
    static int *max_exp = NULL;
    static size_t allocated = 0;

    if (max_exp == NULL || allocated < (size_t)primes_cnt) {
        /* allocate or reallocate */
        if (max_exp) free(max_exp);
        max_exp = (int *)malloc((size_t)primes_cnt * sizeof(int));
        if (!max_exp) return (seq_t)0;   /* out of memory */
        allocated = (size_t)primes_cnt;
    }

    /* reset exponents for a fresh computation */
    for (size_t i = 0; i < allocated; ++i) max_exp[i] = 0;

    /* iterate through prime gaps 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* gap between primes[k] and primes[k-1] (1‑based indices) */
        uint32_t gap = primes[(size_t)k] - primes[(size_t)(k - 1)];
        if (gap == 0) continue;  /* should not happen */

        /* factorise the current gap into prime indices and exponents */
        int f_idx[10];   /* holds indices of primes in the factorisation */
        int f_exp[10];
        int f_cnt = 0;
        uint32_t rem = gap;

        for (int pi = 0; pi < primes_cnt && (uint32_t)primes[pi] * (uint32_t)primes[pi] <= rem; ++pi) {
            if (rem % primes[pi] == 0) {
                int e = 0;
                while (rem % primes[pi] == 0) {
                    rem /= primes[pi];
                    ++e;
                }
                f_idx[f_cnt] = pi;
                f_exp[f_cnt] = e;
                ++f_cnt;
            }
        }
        if (rem > 1) {  /* remaining prime factor */
            int pi = prime_index_of(rem);
            if (pi >= 0) {
                f_idx[f_cnt] = pi;
                f_exp[f_cnt] = 1;
                ++f_cnt;
            }
        }

        /* compute a(k) = LCM(prev) / gcd(LCM(prev), gap) */
        seq_t ak = (seq_t)1;
        for (size_t pi = 0; pi < allocated; ++pi) {
            if (max_exp[pi] == 0) continue;
            int e_in_gap = 0;
            for (int fi = 0; fi < f_cnt; ++fi) {
                if (f_idx[fi] == (int)pi) {
                    e_in_gap = f_exp[fi];
                    break;
                }
            }
            int exp_use = max_exp[pi] - e_in_gap;
            if (exp_use > 0) {
                seq_t factor = pow_int_seq((seq_t)primes[pi], (idx_t)exp_use);
                ak *= factor;
            }
        }

        /* if this is the requested n, return */
        if (k == n) return ak;

        /* update LCM exponents */
        for (int fi = 0; fi < f_cnt; ++fi) {
            int pi = f_idx[fi];
            if (f_exp[fi] > max_exp[pi]) max_exp[pi] = f_exp[fi];
        }
    }

    /* should not reach here for n>=1 */
    return (seq_t)0;
}
