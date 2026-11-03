/*
 * A215383  Primes congruent to {1, 2, 3, 6} mod 13.
 * https://oeis.org/A215383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 972 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime congruent to 1, 2, 3 or 6 (mod 13). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 – behaviour for n<1 is irrelevant */

    /* Cache of already‑found primes */
    static seq_t *primes = NULL;
    static size_t len = 0;           /* number of primes stored */
    static size_t cap = 0;           /* capacity of the array (used by append_candidate) */
    static idx_t cand = 2;           /* next integer to test */

    while (len < n) {
        if (is_prime(cand)) {
            idx_t r = cand % 13;
            if (r == 1 || r == 2 || r == 3 || r == 6) {
                append_candidate(&primes, &len, &cap, (seq_t)cand);
            }
        }
        ++cand;
    }
    return primes[n - 1];
}
