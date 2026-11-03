/*
 * A154621  Primes congruent to 32 mod 67.
 * https://oeis.org/A154621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computes the nth prime congruent to 32 (mod 67)
 * Range:     u8 n <= 1, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 971 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static cache of computed terms */
static seq_t *a_cache = NULL;    /* array of seq_t values */
static size_t  a_cap   = 0;      /* current allocated capacity */
static idx_t   a_len   = 0;      /* number of terms stored so far */

/* next k such that 32 + 67*k is a candidate,
   starting at k = 3 gives the first term 233 */
static idx_t next_k = 3;

/* Ensure that a_cache contains at least n terms (a(n) is 1‑based). */
static void
ensure_n_terms(idx_t n)
{
    while (a_len < n) {
        /* candidate number in the arithmetic progression */
        idx_t cand_idx = 32 + 67 * next_k;
        /* convert to seq_t for storage while testing primality */
        seq_t cand = (seq_t)cand_idx;

        /* test primality */
        if (is_prime(cand_idx)) {
            /* grow the cache if necessary */
            if (a_len >= a_cap)
                grow_seq(&a_cache, &a_cap, a_len + 1);

            a_cache[a_len++] = cand;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    /* Offset of OEIS is 1, so n >= 1 */
    if (n == 0) {
        /* Undefined for n==0; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* make sure we have computed enough terms */
    ensure_n_terms(n);

    /* return the stored value (0‑based index) */
    return a_cache[n - 1];
}
