/*
 * A068819  n!/((n+1)*(n+2)*...*(n+k)) where k is largest value that gives an
 *          integer quotient.
 * https://oeis.org/A068819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n! / ((n+1)(n+2)...(n+k)) where k is maximal such that
 *            the product (n+1)...(n+k) divides n!
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   exponent_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/exponent_in_factorial.h"

#ifdef _MSC_VER
/* For the MSVC compiler use the usual type definitions */
typedef unsigned long long uint64_t;
typedef unsigned long  size_t;
#endif

/* static prime tables up to MAX_PRIME */
#define MAX_PRIME 20000
#define MAX_PRIMES 2500
static int prime_list[MAX_PRIMES];
static int prime_index[MAX_PRIME + 1];
static int prime_count = 0;
static bool primes_initialized = false;

/* initialize primes once */
static void init_primes(void) {
    if (primes_initialized) return;
    /* default all indices to -1 */
    for (int i = 0; i <= MAX_PRIME; ++i) prime_index[i] = -1;
    bool is_composite[MAX_PRIME + 1] = {0};
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            if (prime_count < MAX_PRIMES) {
                prime_list[prime_count++] = i;
                prime_index[i] = prime_count - 1;
            }
            if ((int64_t)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i) {
                    is_composite[j] = 1;
                }
            }
        }
    }
    primes_initialized = true;
}

/* fast integer power for seq_t (works for any seq_t type) */
static seq_t pow_seq_t(seq_t base, int exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 1;  /* not used because offset=1, but safe */

    init_primes();

    /* factorial prime exponents for this n */
    int exps[MAX_PRIMES] = {0};
    int prodExp[MAX_PRIMES] = {0};   /* exponents of product (n+1)... as we grow */

    /* compute exponents of n! for primes <= n */
    for (int i = 0; i < prime_count; ++i) {
        int p = prime_list[i];
        if (p > (int)n) break;
        exps[i] = exponent_in_factorial((uint64_t)n, p);
    }

    /* iterative search for maximal k */
    idx_t i = n + 1;
    int idx[10], cnt[10];   /* temporary storage for factorization (max primes per i is very small) */
    for (;;) {
        uint64_t val = (uint64_t)i;
        int sz = 0;
        bool divisible = true;

        /* factor i by small primes */
        for (int j = 0; j < prime_count && (uint64_t)prime_list[j] * prime_list[j] <= val; ++j) {
            int p = prime_list[j];
            int c = 0;
            while (val % p == 0) { ++c; val /= p; }
            if (c) {
                if (p > (int)n) { divisible = false; break; }
                int id = prime_index[p];
                if (prodExp[id] + c > exps[id]) { divisible = false; break; }
                idx[sz] = id; cnt[sz] = c; ++sz;
            }
        }
        if (divisible && val > 1) {  /* remaining prime factor */
            if (val > (uint64_t)n) { divisible = false; }
            else {
                int id = prime_index[(int)val];
                if (prodExp[id] + 1 > exps[id]) { divisible = false; }
                else { idx[sz] = id; cnt[sz] = 1; ++sz; }
            }
        }

        if (!divisible) break;   /* cannot extend further */

        /* accept this i, update product exponents */
        for (int t = 0; t < sz; ++t) prodExp[idx[t]] += cnt[t];
        ++i;
    }

    /* construct result: exps - prodExp */
    seq_t result = 1;
    for (int j = 0; j < prime_count; ++j) {
        int diff = exps[j] - prodExp[j];
        if (diff > 0) result *= pow_seq_t((seq_t)prime_list[j], diff);
    }

    return result;
}
