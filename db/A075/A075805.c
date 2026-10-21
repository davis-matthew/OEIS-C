/*
 * A075805  Differences between adjacent palindromic numbers which are
 *          products of an even number of distinct primes.
 * https://oeis.org/A075805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = difference between the (n+1)-th and n-th palindromic
 *            numbers that are square‑free products of an even number of
 *            distinct primes
 * Range:     u8 n <= 9, u16 n <= 266, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, is_palindrome, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_palindrome.h"
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

/* Global cache of qualifying palindromic numbers. */
static seq_t *pal_list = NULL;     /* array of palindromic numbers */
static size_t pal_cap  = 0;       /* allocated capacity of pal_list  */
static size_t pal_len  = 0;       /* number of elements currently stored */

/* The next integer to test for palindromicity and the required property. */
static uint64_t next_candidate = 1;

/* Ensure that pal_list has at least `needed` slots. */
static void ensure_capacity(size_t needed)
{
    if (needed <= pal_cap) return;

    size_t new_cap = pal_cap ? pal_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(*new_arr));
    if (pal_list) {
        for (size_t i = 0; i < pal_len; ++i) new_arr[i] = pal_list[i];
        free(pal_list);
    }
    pal_list = new_arr;
    pal_cap  = new_cap;
}

/* Test whether n is a product of an even number of distinct primes
   and is square‑free (i.e. no repeated prime factors). */
static bool good_candidate(uint64_t n)
{
    if (n == 0) return false;
    return is_squarefree((seq_t)n) && ((count_distinct_primes(n) & 1) == 0);
}

/* Generate qualifying palindromic numbers until we have at least
   indices 0 .. n (i.e. pal_len > n). */
static void generate_more(size_t n)
{
    while (pal_len <= n) {
        uint64_t x = next_candidate++;
        if (!is_palindrome((seq_t)x, 10)) continue;
        if (!good_candidate(x)) continue;
        ensure_capacity(pal_len + 1);
        pal_list[pal_len++] = (seq_t)x;
    }
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 0 is undefined for this sequence */
    generate_more((size_t)n);
    return pal_list[(size_t)n] - pal_list[(size_t)n - 1];
}
