/*
 * A199818  Odd numbers producing 20 even numbers in the Collatz iteration.
 * https://oeis.org/A199818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Enumerates odd starting values producing exactly 20 even terms
 *            in the Collatz trajectory
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Lazily compute terms up to the requested index.
       Results are stored in a static dynamic array. */
    static seq_t *terms = NULL;   /* array of found odd numbers */
    static size_t terms_cap = 0;  /* capacity of the array */
    static size_t terms_len = 0;  /* number of terms stored */

    static idx_t next_candidate = 1;  /* next odd number to test */

    /* Grow the array as needed */
    if (!terms_cap) {
        terms_cap = 8;
        terms = (seq_t *)realloc(NULL, terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        seq_t cand = next_candidate;
        next_candidate += 2;  /* only odd numbers */

        /* Count evens in the Collatz trajectory of cand */
        seq_t x = cand;
        idx_t evens = 0;
        while (x != 1 && evens <= 20) {
            if ((x & 1) == 0) {
                evens++;            /* x is even: count it */
                x = x / 2;           /* next term */
            } else {
                /* x is odd: 3x+1, use 128‑bit to avoid overflow on 64‑bit builds */
                unsigned __int128 tmp = (unsigned __int128)x * 3 + 1;
                x = (seq_t)tmp;
            }
        }

        if (evens == 20) {
            if (terms_len == terms_cap) {
                size_t newcap = terms_cap << 1;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* In an unlikely OOM scenario, bail. */
                    return 0;
                }
                terms = tmp;
                terms_cap = newcap;
            }
            terms[terms_len++] = cand;
        }
    }

    return terms[n - 1];
}
