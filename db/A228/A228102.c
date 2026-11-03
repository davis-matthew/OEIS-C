/*
 * A228102  Numbers n such that sum of all primes <=n is prime.
 * https://oeis.org/A228102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 105, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 972 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228102: numbers n such that the sum of all primes ≤ n is prime.
   The function caches the sequence, so calls in any order are efficient. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n)
{
    /* Cached terms of the sequence */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;

    /* Current state of the search: last integer examined
       and cumulative sum of primes up to it. */
    static idx_t current_n = 1;   /* last processed integer */
    static seq_t sum = 0;          /* sum of all primes ≤ current_n */

    /* Extend the cache until we have at least n terms */
    while (terms_len < n) {
        /* Move to the next integer */
        current_n++;

        /* If current_n is prime, add it to the running sum */
        if (is_prime(current_n))
            sum = sum + (seq_t)current_n;

        /* If the cumulative sum is prime, current_n is a term */
        if (is_prime_big(sum)) {
            /* Grow the cache if necessary */
            if (terms_len + 1 > terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp)
                    abort();          /* memory allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)current_n;
        }
    }

    return terms[n - 1];
}
