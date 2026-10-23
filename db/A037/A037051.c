/*
 * A037051  Palindromic product of some n with sum of next n consecutive
 *          integers.
 * https://oeis.org/A037051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   a(n) = n^2 * (3n + 1) / 2 where n is the n‑th integer such that
 *            this value is a palindrome in base 10. The sequence returns the
 *            sequence of those palindromic products
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_palindrome_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome_seq.h"

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of terms cached */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_n = 1;         /* next n to test for palindromic product */

/* Ensure cache has enough space for at least "need" elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for zero, return 0 */

    /* Compute terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t N = (seq_t)next_n;
        seq_t val = N * N;                            /* N^2 */
        val = val * ((seq_t)3 * N + (seq_t)1);        /* N^2 * (3N+1) */
        val = val / (seq_t)2;                        /* divide by 2 */

        if (is_palindrome_seq(val)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_n;
    }

    return cache[n - 1];
}
