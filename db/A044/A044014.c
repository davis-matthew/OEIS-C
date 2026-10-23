/*
 * A044014  Numbers k such that 2 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A044014 – numbers k such that the decimal representation of
 *  k contains the adjacent digits 24 or 42 but the decimal representation
 *  of k+1 does not.  The function returns the n‑th such number (1‑based). */

#include <stdlib.h>
#include <stdbool.h>

/* Return TRUE if n has an adjacent pair 24 or 42. */
static bool has_adjacent_pair(seq_t n)
{
    seq_t prev = 0;
    bool first = true;
    while (n > 0) {
        seq_t d = n % (seq_t)10;
        if (!first) {
            if ((prev == (seq_t)2 && d == (seq_t)4) ||
                (prev == (seq_t)4 && d == (seq_t)2))
                return true;
        }
        prev = d;
        first = false;
        n /= (seq_t)10;
    }
    return false;
}

/*  a(idx_t n) – return the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) {               /* undefined for n==0, return 0 as safe fallback */
        return (seq_t)0;
    }

    /* ----- static storage ---------------------------------------------- */
    static seq_t *cache = NULL;   /* array of found terms                */
    static size_t cache_size = 0; /* how many terms are stored in cache  */
    static size_t cache_cap  = 0; /* allocated capacity of cache         */
    static seq_t next_k = 1;      /* next candidate number to test        */

    /* ----- grow until we have at least n terms -------------------------- */
    while (cache_size < (size_t)n) {

        /* Search for the next valid k. */
        for (;;) {
            if (has_adjacent_pair(next_k) && !has_adjacent_pair(next_k + (seq_t)1)) {
                /* Ensure space. */
                if (cache_size >= cache_cap) {
                    size_t new_cap = cache_cap ? 2 * cache_cap : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (tmp == NULL) {
                        /* Abort if allocation fails. */
                        exit(EXIT_FAILURE);
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = next_k;
                next_k++;               /* next search starts after this term */
                break;
            }
            next_k++;
        }
    }

    return cache[(size_t)n - 1];
}
