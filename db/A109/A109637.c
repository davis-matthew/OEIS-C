/*
 * A109637  Numbers n such that the string 55n is prime.
 * https://oeis.org/A109637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer n for which 55 * 10^digits(n) + n is
 *            prime
 * Range:     u8 none, u16 n <= 112, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 70 terms through n = 1985 (2026-11-03)
 * Helpers:   is_prime_seq, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

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

seq_t a(idx_t n)
{
    /* Static cache of discovered terms */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;
    static idx_t cache_size  = 0;
    static idx_t last_checked = 0;

    /* Return stored value if already computed */
    if (cache_size >= n)
        return cache[n - 1];

    /* Extend cache as needed */
    while (cache_size < n)
    {
        /* Next candidate integer */
        idx_t cand = last_checked + 1;
        last_checked = cand;

        /* Number of decimal digits in cand */
        idx_t d = num_digits_idx(cand);

        /* Candidate number 55...n (concatenation) */
        seq_t m = (seq_t)55 * pow10(d) + (seq_t)cand;

        /* Check primality */
        if (is_prime_seq(m))
        {
            push_seq(&cache, &cache_cap, &cache_size, (seq_t)cand);
        }
    }

    return cache[n - 1];
}
