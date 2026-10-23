/*
 * A032715  Numbers k such that k prefixed by '7' and followed by '3' is
 *          prime.
 * https://oeis.org/A032715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 29, u32 n <= 29, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032715: numbers k such that 7 followed by k then 3 is prime. */
/* The value a(n) is the n‑th such integer, 1‑based.  The function uses a
 * simple incremental search with memoisation; it is correct for all n
 * provided the auxiliary functions compile under C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/is_prime_big.h"

/* Public API ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Cache of already discovered terms (k values). */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Next candidate k to test. */
    static idx_t current_k   = 1;

    /* Allocate initial cache if this is the first call. */
    if (cache == NULL) {
        cache_cap = 64;
        /* Use a C‑style cast so the code compiles as both C and C++. */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t k = current_k++;                /* candidate k */
        int d   = digits(k);                  /* decimal digits of k */

        /* Construct N = 7 * 10^(d+1) + k*10 + 3 as a seq_t. */
        seq_t p = pow10((idx_t)(d + 1));      /* 10^(d+1) */
        seq_t N = 7 * p + ((seq_t)k * 10 + 3);

        /* Test primality. */
        if (is_prime_big(N)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return (seq_t)0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    /* Return the n‑th term (1‑based). */
    return cache[(size_t)n - 1];
}
