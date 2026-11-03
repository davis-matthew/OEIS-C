/*
 * A070805  a(1) = 2; a(n) = largest prime not exceeding the sum of all
 *          previous terms.
 * https://oeis.org/A070805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3; for n>=3, a(n) = largest prime not exceeding the
 *            sum of all previous terms
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *terms = NULL;      /* dynamically allocated array of terms */
static idx_t  max_n  = 0;        /* highest index that has been computed */
static seq_t  sum    = 0;        /* sum of terms[1..max_n] */

/* Find the largest prime <= s */
static seq_t prev_prime_leq(seq_t s)
{
    if (s < 2) return 2;
    seq_t cand = s;
    if (cand > 2 && (cand & 1) == 0) cand -= 1;   /* make it odd if >2 */
    while (true) {
        if (is_prime_big(cand)) return cand;
        if (cand == 2) return 2;                  /* fallback */
        cand -= 2;                                 /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* Undefined; return 0 for safety */

    /* Allocate an array large enough for the current request */
    if (terms == NULL) {
        terms = seq_alloc_new_array((size_t)n + 1); /* index 0 unused */
        if (terms == NULL) return 0;              /* Allocation failed */
        for (size_t i = 0; i <= (size_t)n; ++i) terms[i] = 0;
        sum   = 0;
        max_n = 0;
    }

    /* Grow the array if necessary */
    if (n > max_n) {
        seq_t *new_terms = seq_alloc_new_array((size_t)n + 1);
        if (new_terms == NULL) return 0;         /* Allocation failed */

        for (idx_t i = 1; i <= max_n; ++i) new_terms[i] = terms[i];
        seq_free_array(terms);
        terms = new_terms;

        /* Compute new terms */
        if (max_n == 0) {                /* initialise a(1) */
            terms[1] = 2;
            sum = 2;
            max_n = 1;
        }
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prime;
            if (i == 2) {
                prime = 3;                        /* special case */
            } else {
                prime = prev_prime_leq(sum);
            }
            terms[i] = prime;
            sum += prime;
        }
        max_n = n;
    }
    return terms[n];
}
