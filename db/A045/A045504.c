/*
 * A045504  Palindromic Fibonacci numbers.
 * https://oeis.org/A045504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more, hard
 * Formula:   a(n) is the n‑th Fibonacci number that is a palindrome in base
 *            10. The sequence terminates after 55
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"      /* bool is_palindrome(seq_t, idx_t) */

/* Cache of discovered palindromic Fibonacci numbers. */
static seq_t    *pal_cache = NULL;
static size_t    pal_len   = 0;
static size_t    pal_cap   = 0;

/* Current Fibonacci state for incremental generation. */
static idx_t    fib_idx   = 0;          /* index of fib_prev   */
static seq_t    fib_prev  = 0;          /* F(fib_idx)          */
static seq_t    fib_curr  = 1;          /* F(fib_idx+1)        */

/* Ensure the cache can store one more element. */
static void add_palind(seq_t v)
{
    if (pal_len == pal_cap) {
        size_t new_cap = pal_cap ? pal_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(pal_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – the sequence is finite, exit safely. */
            exit(1);
        }
        pal_cache = tmp;
        pal_cap   = new_cap;
    }
    pal_cache[pal_len++] = v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)    /* the archive offset is 1, so n=0 is outside the domain */
        return 0;

    /* Generate Fibonacci numbers until we have found n palindromic ones
       or until a practical search limit is reached. */
    /* The sequence is known to terminate; we use a generous but finite
       iterator bound to avoid infinite loops. */
    const idx_t MAX_FIB_INDEX = 1000000;   /* 1 M Fibonacci indices are enough */
    while (pal_len < (size_t)n && fib_idx < MAX_FIB_INDEX) {
        if (is_palindrome(fib_prev, (idx_t)10))
            add_palind(fib_prev);

        /* Advance to the next Fibonacci number. */
        fib_idx++;
        seq_t tmp = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = tmp;
    }

    /* If the required term was not discovered, the sequence is exhausted. */
    if (pal_len < (size_t)n)
        return 0;

    return pal_cache[n - 1];
}
