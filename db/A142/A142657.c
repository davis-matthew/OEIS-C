/*
 * A142657  Primes congruent to 39 mod 56.
 * https://oeis.org/A142657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 39 (mod 56)
 * Range:     u8 n <= 1, u16 n <= 254, u32 n <= 266, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* The build supplies seq_t and idx_t.  */

/* Cache of primes already found.  Stored in the order they appear. */
static seq_t *class_primes = NULL;
static size_t class_cnt   = 0;   /* number of stored class primes   */
static size_t class_cap   = 0;   /* allocated capacity              */
static bool   initialized = false;
/* next_k such that 56 * next_k + 39 is the next candidate to test.  */
static idx_t next_k = 0;

/* Ensure the cache has at least one free slot. */
static void ensure_class_capacity(void)
{
    if (class_cnt >= class_cap) {
        size_t new_cap = class_cap ? (class_cap << 1) : 16;
        seq_t *tmp = (seq_t *)realloc(class_primes, new_cap * sizeof(seq_t));
        class_primes = tmp;
        class_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* In OEIS, offset = 1; behavior for n < 1 is irrelevant. */
    if (n == 0) return (seq_t)0;

    if (!initialized) {
        /* First call: initialize state. */
        initialized = true;
    }

    /* Keep generating candidates until we have at least n class primes. */
    while (class_cnt < n) {
        idx_t cand_idx = next_k++;                 /* 0‑based index   */
        idx_t cand     = 56 * cand_idx + 39;        /* candidate value */

        /* Test for primality.  The helper works for the current seq_t. */
        if (is_prime_big((seq_t)cand)) {
            ensure_class_capacity();
            class_primes[class_cnt++] = (seq_t)cand;
        }
    }

    return class_primes[n - 1];
}
