/*
 * A061239  Prime numbers == 4 (mod 9).
 * https://oeis.org/A061239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers == 4 (mod 9).  a(n) is the nth prime ≡ 4 (mod 9). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap > 0) ? *cap : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failure: caller will return 0 */
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out‑of‑range */

    /* Cached terms */
    static seq_t *terms        = NULL;
    static size_t terms_len    = 0;      /* number of stored terms   */
    static size_t terms_cap    = 0;      /* current capacity        */
    static idx_t next_candidate = 2;     /* next integer to test    */

    /* Build the table up to the requested index. */
    while (terms_len < n) {
        /* Move to next integer and test it. */
        next_candidate++;                       /* next trial number    */

        if (is_prime(next_candidate) &&
            (next_candidate % 9) == 4) {        /* prime and ≡4 (mod 9) */
            /* Ensure enough room and store the prime. */
            grow_seq_t_array(&terms, &terms_cap, terms_len + 1);
            if (!terms) return 0;               /* allocation failure   */
            terms[terms_len++] = (seq_t)next_candidate;
        }
    }

    return terms[n - 1];          /* 1‑based index */
}
