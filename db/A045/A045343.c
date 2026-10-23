/*
 * A045343  Primes congruent to {2, 3} mod 7.
 * https://oeis.org/A045343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 2 or 3 (mod 7)
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* Cache of primes found so far.  tbl[0] = 2, the first prime ≡ 2 (mod 7). */
static seq_t *tbl   = NULL;   /* dynamic array of primes */
static size_t tbl_len = 0;     /* number of primes stored */
static size_t tbl_cap = 0;     /* allocated capacity of tbl */

/* Candidate odd number to test next.  Starts at 3. */
static uint64_t cand = 3;

/* Initialise the cache with the first prime. */
static void init_tbl(void) {
    if (!tbl) {
        tbl_cap = 16;
        tbl = (seq_t *)malloc(tbl_cap * sizeof(seq_t));
        tbl[0] = (seq_t)2;      /* 2 ≡ 2 (mod 7) */
        tbl_len = 1;
    }
}

/* Grow the cache until it contains at least `n` entries. */
static void ensure_up_to(idx_t n) {
    init_tbl();

    while (tbl_len < n) {
        if (is_prime_big((seq_t)cand)) {
            if ((cand % 7 == 2) || (cand % 7 == 3)) {
                if (tbl_len == tbl_cap) {
                    tbl_cap <<= 1;
                    tbl = (seq_t *)realloc(tbl, tbl_cap * sizeof(seq_t));
                }
                tbl[tbl_len++] = (seq_t)cand;
            }
        }
        cand += 2;   /* next odd candidate */
    }
}

/* Return the n‑th prime congruent to 2 or 3 modulo 7 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but harmless */
    ensure_up_to(n);
    return tbl[n - 1];
}
