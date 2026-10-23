/*
 * A137920  Numbers k such that 24*k-1 and 24*k+1 are twin primes.
 * https://oeis.org/A137920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 755, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137920: numbers k such that 24*k-1 and 24*k+1 are twin primes. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static buffer holding all terms discovered so far. */
    static seq_t *memo = NULL;        /* array of seq_t */
    static size_t memo_cap = 0;       /* current allocation capacity */
    static size_t memo_len = 0;       /* number of terms stored (memo_len ≥ 1 for all valid n) */
    static seq_t next_k = 1;          /* next candidate k to test */

    if (n == (idx_t)0)               /* undefined for offset 0 */
        return 0;

    /* Extend the memo until it contains at least n terms */
    while (memo_len < (size_t)n) {
        seq_t k = next_k;
        seq_t found_k = 0;

        /* Search forward until we find the next k satisfying the twin‑prime condition. */
        for (;;) {
            seq_t val1 = (seq_t)24 * k - (seq_t)1;
            seq_t val2 = (seq_t)24 * k + (seq_t)1;

            /* is_prime operates on idx_t (unsigned 64‑bit).  The values we test are
               far below 2^64 for the ranges required by the tests, so a cast is safe. */
            if (is_prime((idx_t)val1) && is_prime((idx_t)val2)) {
                found_k = k;
                break;
            }
            k = k + (seq_t)1;          /* try the next integer */
        }

        /* Append the found term to the memo buffer, reallocating if necessary. */
        if (memo_len == memo_cap) {
            size_t new_cap = memo_cap ? memo_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) return 0;       /* allocation failure – return 0 as a safe value */
            memo = tmp;
            memo_cap = new_cap;
        }
        memo[memo_len++] = found_k;

        /* Prepare for the next search. */
        next_k = found_k + (seq_t)1;
    }

    /* The array is zero‑based, whereas the OEIS offset is 1. */
    return memo[n - (idx_t)1];
}
