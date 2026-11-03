/*
 * A201804  Numbers k such that 90*k + 11 is prime.
 * https://oeis.org/A201804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9903 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th non‐negative integer k such that 90*k + 11 is prime
 *  (OEIS A201804).  The routine builds the sequence on demand and
 *  remembers the values so that repeated calls in any order stay fast. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n) {
    /* static cache for the sequence values */
    static seq_t *seq = NULL;      /* array of computed k's */
    static size_t seq_cap = 0;     /* current allocation capacity */
    static size_t seq_len = 0;     /* number of elements stored */
    static idx_t cur_k = 0;        /* next k to test for primality */

    if (n == 0) return 0;          /* undefined for offset 1, safe fallback */

    /* create storage on first use */
    if (seq == NULL) {
        seq_cap = 16;
        seq = seq_alloc_new_array(seq_cap);
    }

    /* generate values until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find the next k with 90*k + 11 prime */
        while (1) {
            /* candidate value to test for primality */
            seq_t val = (seq_t)(90 * cur_k + 11);

            if (isPrimeGeneric(val)) {
                /* enlarge storage if necessary */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = seq_alloc_new_array(new_cap);
                    for (size_t i = 0; i < seq_len; ++i)
                        tmp[i] = seq[i];
                    free_seq(seq);
                    seq = tmp;
                    seq_cap = new_cap;
                }
                /* store the found k */
                seq[seq_len++] = (seq_t)cur_k;
                cur_k++;
                break;          /* move on to next required term */
            }
            cur_k++;            /* try next k */
        }
    }

    /* return the requested term (1‑based indexing) */
    return seq[n - 1];
}
