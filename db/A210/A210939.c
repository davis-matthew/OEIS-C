/*
 * A210939  Nonprime nearest-neighbors of the primes.
 * https://oeis.org/A210939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nonprime nearest-neighbor of primes (OEIS A210939)
   The function finds composite numbers that have at least one
   adjacent prime (either n‑1 or n+1).  The sequence is generated
   incrementally and cached for repeated calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *seq_arr   = NULL;   /* cached sequence values          */
static size_t seq_cap   = 0;      /* capacity of seq_arr             */
static size_t seq_len   = 0;      /* number of values stored         */
static idx_t next_to_check = 1;   /* next integer to examine         */

/* Grow the cache array when capacity is exhausted. */
static void seq_grow(void)
{
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    seq_t *new_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* Generate values until we have at least ‘n’ terms. */
static void generate_until(idx_t n)
{
    while ((idx_t)seq_len < n) {
        idx_t x = next_to_check;

        /* x is non‑prime and at least one of x−1 or x+1 is prime */
        bool nonprime = !is_prime(x);
        bool neigh_prime = false;
        if (x > 1 && is_prime(x - 1)) neigh_prime = true;
        if (is_prime(x + 1))          neigh_prime = true;

        if (nonprime && neigh_prime) {
            if (seq_len == seq_cap) seq_grow();
            seq_arr[seq_len++] = (seq_t)x;
        }
        ++next_to_check;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;       /* undefined for 0, but safe to return 0 */
    generate_until(n);
    return seq_arr[n - 1];
}
