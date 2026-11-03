/*
 * A082509  Differences between consecutive primes that are not powers of 2 in
 *          order of their appearance. Differences which are powers of 2 are
 *          omitted from A001223.
 * https://oeis.org/A082509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9197 (2026-11-03)
 * Helpers:   is_pow2_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes differences between consecutive primes that are NOT powers of 2.
   Returns the n-th such difference (1‑based). The sequence is OEIS A082509. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_pow2_u64.h"
#include "../utils/mem.h"

/* Grow an array of seq_t elements. */
static void append_seq (seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* Grow an array of uint64_t elements. */
static void append_u64 (uint64_t **arr, size_t *len, size_t *cap, uint64_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* Cached data between calls. */
static uint64_t *prime_list   = NULL;
static size_t   prime_len    = 0;
static size_t   prime_cap    = 0;

static seq_t   *diff_list    = NULL;
static size_t   diff_len     = 0;
static size_t   diff_cap     = 0;

static uint64_t next_candidate = 3;     /* start after the first known prime 3 */

/* @param n ≥ 1
   @return the n-th term of A082509 */
seq_t a (idx_t n)
{
    /* Ensure at least n terms are ready. */
    while (diff_len < (size_t)n) {

        /* Find the next prime ≥ next_candidate. */
        uint64_t p = next_candidate;
        for (;;) {
            bool prime = true;
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t q = prime_list[i];
                if ((unsigned __int128)q * q > p) break;
                if (p % q == 0) { prime = false; break; }
            }
            if (prime) break;
            p += 2;                           /* skip even numbers */
        }

        /* Store the new prime. */
        append_u64(&prime_list, &prime_len, &prime_cap, p);

        /* Record the difference to the previous prime, if any. */
        if (prime_len >= 2) {
            uint64_t diff = p - prime_list[prime_len - 2];
            if (!is_pow2_u64(diff))
                append_seq(&diff_list, &diff_len, &diff_cap, (seq_t)diff);
        }

        /* Advance candidate for next search. */
        next_candidate = p + 2;
    }

    return diff_list[n - 1];
}
