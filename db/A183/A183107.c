/*
 * A183107  Sum of palindromic divisors of numbers k such that the sum of the
 *          palindromic divisors of k is palindromic.
 * https://oeis.org/A183107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of palindromic divisors of the n‑th number k such
 *            that the sum of its palindromic divisors is also a palindrome.
 *            a(n) = A088000(A183106(n)). The implementation scans natural
 *            numbers k, keeps those with a palindromic sum of palindromic
 *            divisors, and stores the corresponding sums as the...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_palindrome, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_palindrome.h"          /* is_palindrome(seq_t x, idx_t base) */
#include "../utils/seq_alloc_new_array.h"    /* seq_alloc_new_array(size_t) */
#include "../utils/seq_free_array.h"         /* seq_free_array(seq_t*) */

/* static cache of sequence values a(1), a(2), …  */
static seq_t *term_cache = NULL;     /* pointer to array of seq_t */
static size_t cache_size = 0;        /* number of terms stored   */
static size_t cache_cap  = 0;        /* allocated capacity       */

/* next integer k to test for the property */
static idx_t next_k = 1;


/* Ensure the cache has at least 'needed' slots allocated.  */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_arr = seq_alloc_new_array(new_cap);
    if (!new_arr) {                       /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }

    /* copy old contents */
    for (size_t i = 0; i < cache_size; ++i)
        new_arr[i] = term_cache[i];

    /* free old array (if any) */
    if (term_cache)
        seq_free_array(term_cache);

    term_cache = new_arr;
    cache_cap  = new_cap;
}


/* Return the n-th term of A183107;  n >= 1.
   For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Grow cache until it contains at least n terms. */
    ensure_cache_capacity((size_t)n);
    while (cache_size < (size_t)n)
    {
        idx_t k = next_k++;
        seq_t sum = (seq_t)0;

        /* enumerate divisors d of k up to sqrt(k) */
        for (uint64_t d = 1; d * d <= (uint64_t)k; ++d)
        {
            if (k % d == 0)
            {
                seq_t sd = (seq_t)d;
                if (is_palindrome(sd, 10))
                {
                    sum += sd;
                }

                uint64_t e = k / d;
                if (e != d)          /* distinct divisor pair */
                {
                    seq_t se = (seq_t)e;
                    if (is_palindrome(se, 10))
                    {
                        sum += se;
                    }
                }
            }
        }

        /* If the sum of palindromic divisors is itself a palindrome,
           record it as the next sequence value. */
        if (is_palindrome(sum, 10))
        {
            term_cache[cache_size++] = sum;
        }
    }

    return term_cache[n - 1];  /* index 0 corresponds to a(1) */
}
