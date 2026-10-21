/*
 * A075072  a(1) = 1; for n > 1, a(n) = n! divided by product of factorials of
 *          all prime divisors of n.
 * https://oeis.org/A075072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = n! / ∏_{p|n} p! (product over distinct prime divisors of
 *            n)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Helpers:   exp_in_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/exp_in_fact.h"

/* maximum n expected in the tests (safe upper bound for 10000) */
#define MAX_N 20000
#define MAX_PRIME_CNT 2300   /* number of primes ≤ MAX_N */

/* global sieve data */
static uint64_t global_primes[MAX_PRIME_CNT];
static size_t   global_prime_cnt = 0;
static bool     global_primes_inited = false;

/* initialise the prime list once – called by a() */
static void init_primes(void)
{
    if (global_primes_inited) return;
    unsigned char *mark = (unsigned char *)calloc(MAX_N + 1, 1);
    if (!mark) return;                           /* unlikely, but keep safe */

    for (uint64_t i = 2; i <= MAX_N; ++i) {
        if (!mark[i]) {
            global_primes[global_prime_cnt++] = i;
            if (i * i <= MAX_N) {
                for (uint64_t j = i * i; j <= MAX_N; j += i)
                    mark[j] = 1;
            }
        }
    }
    free(mark);
    global_primes_inited = true;
}

/* fast power for seq_t (works for all builds) */
static inline seq_t pow_seq_local(seq_t base, int exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1)  res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* binary search for a prime index in the global prime array */
static inline size_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = global_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (global_primes[mid] == p) return mid;
        if (global_primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (size_t)-1;  /* not found */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* out‑of‑range defensive guard */
    if (n == 1) return (seq_t)1;

    init_primes();

    uint64_t N = (uint64_t)n;

    /* number of primes ≤ N */
    size_t pi_cnt = 0;
    while (pi_cnt < global_prime_cnt && global_primes[pi_cnt] <= N) ++pi_cnt;
    if (pi_cnt == 0) return (seq_t)1;  /* N==1 case already handled */

    /* exponents of primes in N! */
    static int exps[MAX_PRIME_CNT];
    for (size_t i = 0; i < pi_cnt; ++i)
        exps[i] = exp_in_fact(N, global_primes[i]);

    /* distinct prime divisors of N */
    static size_t primes_of_N[32];
    size_t primes_of_N_cnt = 0;
    uint64_t tmp = N;
    for (size_t i = 0; i < global_prime_cnt && (uint64_t)global_primes[i] * global_primes[i] <= tmp; ++i) {
        if (tmp % global_primes[i] == 0) {
            primes_of_N[primes_of_N_cnt++] = i;
            while (tmp % global_primes[i] == 0) tmp /= global_primes[i];
        }
    }
    if (tmp > 1) {                            /* remaining prime factor > sqrt(N) */
        size_t idx = prime_index_of(tmp);
        if (idx != (size_t)-1) primes_of_N[primes_of_N_cnt++] = idx;
    }

    /* subtract exponents coming from factorials of prime divisors */
    for (size_t di = 0; di < primes_of_N_cnt; ++di) {
        size_t p_idx = primes_of_N[di];
        uint64_t p_val = global_primes[p_idx];
        for (size_t q = 0; q <= p_idx; ++q) {
            exps[q] -= exp_in_fact(p_val, global_primes[q]);
        }
    }

    /* build the result from prime powers */
    seq_t result = (seq_t)1;
    for (size_t q = 0; q < pi_cnt; ++q) {
        int e = exps[q];
        if (e > 0) {
            seq_t base = (seq_t)global_primes[q];
            result *= pow_seq_local(base, e);
        }
    }
    return result;
}
