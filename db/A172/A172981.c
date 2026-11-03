/*
 * A172981  Primes p such that 210*p+41 is also prime.
 * https://oeis.org/A172981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 881 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 210*p+41 is prime */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache for already found terms */
static seq_t *term_cache = NULL;
static size_t term_cap  = 0;
static size_t term_cnt  = 0;

/* Current prime candidate to test */
static seq_t next_candidate_p = 2;

/* Append a new term to the cache, expanding the array as needed */
static void cache_append(seq_t val)
{
    if (term_cnt >= term_cap) {
        term_cap = term_cap ? term_cap * 2 : 16;
        term_cache = mem_resize(seq_t, term_cache, term_cap);
        if (!term_cache) abort();
    }
    term_cache[term_cnt++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */

    /* Generate terms until we have at least n */
    while (term_cnt < n) {
        /* Scan forward until we find the next qualifying prime p */
        while (true) {
            if (is_prime_seq(next_candidate_p)) {
                /* t = 210*p + 41 */
                seq_t cand = next_candidate_p * (seq_t)210 + (seq_t)41;
                if (is_prime_big(cand)) {
                    cache_append(next_candidate_p);
                    /* Move past the found prime for the next search */
                    next_candidate_p++;
                    break;             /* found a term; back to outer loop */
                }
            }
            next_candidate_p++;        /* try the next integer */
        }
    }

    return term_cache[n - 1];
}
