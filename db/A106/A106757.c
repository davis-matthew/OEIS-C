/*
 * A106757  Primes with digit sum = 16.
 * https://oeis.org/A106757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 447, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A106757
 *   Sequence of primes whose digit sum equals 16.
 *   a(n) = the n-th prime p such that sum_of_digits(p) = 16
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"   /* bool is_prime(uint64_t n); */

/* --- helper: digit sum of a 64‑bit integer ----------------------------------- */
static inline int digit_sum_uint64(uint64_t x)
{
    int s = 0;
    while (x != 0) {
        s += (int)(x % 10);
        x /= 10;
    }
    return s;
}

/* --- dynamic array to hold the discovered terms ----------------------------- */
static seq_t   *seq_vals = NULL;   /* array of primes with digit sum 16 */
static idx_t    seq_cap  = 0;       /* current capacity of seq_vals     */
static idx_t    seq_cnt  = 0;       /* number of stored terms           */
static uint64_t cur_test = 1;       /* last integer examined            */

/* Ensure capacity for at least min_needed elements. */
static void ensure_seq_cap(idx_t *cap, seq_t **arr, idx_t min_needed)
{
    if (*cap < min_needed) {
        idx_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap < min_needed) new_cap <<= 1;
        /* realloc may return NULL; let the caller deal with it. */
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Append a new term to the sequence. */
static void add_term(seq_t val)
{
    ensure_seq_cap(&seq_cap, &seq_vals, seq_cnt + 1);
    seq_vals[seq_cnt++] = val;
}

/* Grow the array until it contains at least n terms. */
static void grow_to(idx_t n)
{
    while (seq_cnt < n) {
        cur_test++;                          /* the next integer to test */
        /* skip even numbers greater than 2 */
        if (cur_test > 2u && ((cur_test & 1u) == 0u))
            continue;
        if (is_prime(cur_test) && digit_sum_uint64(cur_test) == 16)
            add_term((seq_t)cur_test);
    }
}

/* Public interface: return the n‑th term.  Indexing is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)       /* undefined for n < 1, but safe fallback */
        return (seq_t)0;
    grow_to(n);
    return seq_vals[n - 1];
}
