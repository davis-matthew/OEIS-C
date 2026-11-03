/*
 * A109603  Numbers n such that 43*n - 41 is prime.
 * https://oeis.org/A109603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer m such that 43*m - 41 is a prime.
   The function generates numbers in increasing order and
   keeps a static list of those m for which 43*m-41 is prime. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n) {
    /* static storage for the list of qualifying n's */
    static seq_t *list = NULL;
    static idx_t sz = 0;
    static idx_t cap = 0;
    /* next n to examine (1-indexed) */
    static uint64_t next_n = 1;

    /* generate until we have at least n elements */
    while (sz < n) {
        /* compute 43*next_n - 41, safe in uint64_t for n up to about 1e12 */
        uint64_t val = 43ULL * next_n - 41ULL;
        if (is_prime_u64(val)) {
            /* store the qualifying n (cast to seq_t) */
            seq_t tmp = next_n;
            push_seq(&list, &cap, &sz, tmp);
        }
        next_n++;   /* move to the next integer to test */
    }

    /* return the (n-1)-th element (0-based) */
    return list[n - 1];
}
