/*
 * A145472  Primes p such that (p+7)/2 is prime.
 * https://oeis.org/A145472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 580, u32 n <= 380, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n-th prime p (1‑based indexing) such that (p+7)/2 is also prime.
 * The value is computed on demand and cached for future calls.
 * The implementation uses a simple incremental search with primality tests provided by the
 * library (`is_prime_big`) and dynamic array management (`seq_realloc`).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t) */
#include "../utils/seq_realloc.h"     /* seq_realloc(seq_t*,size_t,size_t) */

/* --------------------------------------------------------------------- */
/*  Dynamic array of sequence values                                    */
/* --------------------------------------------------------------------- */
static seq_t *seq = NULL;    /* array of found primes */
static size_t seq_len = 0;   /* number of stored terms   */
static size_t seq_cap = 0;   /* current capacity         */
static uint64_t next_candidate = 3;   /* next odd candidate to test */

/* Grow the dynamic array to at least `min_cap` elements. */
static void grow_seq(size_t min_cap)
{
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_ptr = seq_realloc(seq, seq_cap, new_cap);
    seq = new_ptr;
    seq_cap = new_cap;
}

/* Extend the stored sequence until it contains at least `target` terms. */
static void extend_seq(idx_t target)
{
    while (seq_len < (size_t)target) {
        uint64_t p = next_candidate;

        /* Test primality of p */
        seq_t sp = (seq_t)p;
        if (is_prime_big(sp)) {
            /* Compute q = (p+7)/2 and test its primality */
            uint64_t q = (p + 7) >> 1;
            seq_t sq = (seq_t)q;
            if (is_prime_big(sq)) {
                /* store p */
                if (seq_len == seq_cap) grow_seq(seq_cap + 1);
                seq[seq_len++] = sp;
            }
        }

        next_candidate += 2;   /* only odd numbers can be primes >2 */
    }
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* undefined for non‑positive indices */
    extend_seq(n);
    return seq[n - 1];
}
