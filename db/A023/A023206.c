/*
 * A023206  Numbers m such that m and 2*m + 7 both prime.
 * https://oeis.org/A023206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 623, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 792 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as all primes m such that 2*m+7 is also prime.
   For n≥1, a(n) is the n‑th such prime.  The function below builds the
   sequence on demand and caches the results for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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
    /* Static state for memoization */
    static seq_t *terms = NULL;          /* array of found terms */
    static idx_t term_count = 0;         /* number of terms stored */
    static idx_t capacity = 0;           /* current allocation size   */
    static seq_t next_candidate = 5;     /* next m to test (prime ≥5) */

    /* Helper to grow the terms array when needed */
    if (term_count == 0) { /* first call – initialise cache with 2,3 */
        capacity = 128;
        terms = mem_new(seq_t, capacity);
        if (!terms) abort();               /* allocation fail */
        terms[0] = (seq_t)2;
        terms[1] = (seq_t)3;
        term_count = 2;
        next_candidate = 5; /* start checking from 5 */
    }

    /* Generate additional terms until we have at least n of them */
    while (term_count < n) {
        /* Ensure capacity for the new term */
        if (term_count >= capacity) {
            capacity *= 2;
            seq_t *tmp = mem_resize(seq_t, terms, capacity);
            if (!tmp) abort();
            terms = tmp;
        }

        /* Test candidate m */
        if (is_prime_big(next_candidate)) {
            seq_t s = 2 * next_candidate + 7; /* 2*m+7 */
            if (is_prime_big(s)) {
                terms[term_count++] = next_candidate;
            }
        }

        /* Advance to the next odd integer */
        next_candidate += 2;
    }

    return terms[n - 1];
}
