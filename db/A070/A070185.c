/*
 * A070185  Primes p such that x^9 = 2 has a solution mod p, but x^(9^2) = 2
 *          has no solution mod p.
 * https://oeis.org/A070185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 2 is a 9th power residue mod
 *            p, but 2 is NOT an 81st power residue mod p. This uses modular
 *            exponentiation and a simple sieve for prime generation
 * Range:     u8 none, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* ------------------------------------------------------------------------- */
/* helper: simple gcd for small integers                                     */
static int gcd_int(int a, int b)
{
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ------------------------------------------------------------------------- */
/* primes generation (single run)                                            */
static uint32_t *prime_list = NULL;
static size_t     prime_cnt  = 0;
static size_t     prime_cap  = 0;

static void generate_primes(size_t limit)
{
    bool *mark = (bool *)calloc(limit + 1, sizeof(bool));
    if (!mark) { abort(); }

    /* estimate capacity for prime array */
    prime_cap = limit / 10;          /* ~0.1 * limit is a safe overestimate  */
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    prime_cnt = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (!mark[i])
            for (size_t j = i * i; j <= limit; j += i)
                mark[j] = true;

    for (size_t i = 2; i <= limit; ++i)
        if (!mark[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!prime_list) { abort(); }
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    free(mark);
}

/* ------------------------------------------------------------------------- */
/* cache for computed sequence values                                          */
static seq_t *seq_cache   = NULL;
static size_t seq_used    = 0;
static size_t seq_capacity= 0;
static size_t prime_pos   = 0;     /* next prime index to examine */

/* ------------------------------------------------------------------------- */
static bool initialized = false;

seq_t a(idx_t n)
{
    /* first call: generate primes and initialise cache */
    if (!initialized) {
        const size_t limit = 20000000;          /* 20 M covers n ≤ 10 000 comfortably */
        generate_primes(limit);

        seq_capacity = 1024;
        seq_cache = (seq_t *)malloc(seq_capacity * sizeof(seq_t));
        seq_used  = 0;
        prime_pos = 0;
        initialized = true;
    }

    /* generate further terms until we have at least n elements      */
    while (seq_used < (size_t)n) {
        if (prime_pos >= prime_cnt) {            /* exhausted our prime list */
            abort();                              /* shouldn't happen for n ≤ 10 000 */
        }

        uint32_t p = prime_list[prime_pos++];
        if (p <= 2) continue;                    /* skip 2 */

        int g1 = gcd_int(9,  (int)(p - 1));
        int g2 = gcd_int(81, (int)(p - 1));

        uint64_t exp1 = (p - 1) / g1;
        uint64_t exp2 = (p - 1) / g2;

        seq_t r1 = mod_pow((seq_t)2, exp1, (seq_t)p);
        seq_t r2 = mod_pow((seq_t)2, exp2, (seq_t)p);

        if (r1 == (seq_t)1 && r2 != (seq_t)1) {
            if (seq_used == seq_capacity) {
                seq_capacity *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_capacity * sizeof(seq_t));
                if (!seq_cache) { abort(); }
            }
            seq_cache[seq_used++] = (seq_t)p;
        }
    }

    return seq_cache[n - 1];
}
