/*
 * A153343  Numbers k such that 5*k + 4 is not prime.
 * https://oeis.org/A153343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (n‑1)-th nonnegative integer k such that 5*k+4 is
 *            composite (i.e. not prime)
 * Range:     u8 n <= 207, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 969 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* use the helper that guarantees a fast primality test for 64‑bit values */
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Helper to grow an array of seq_t (defined by the build) */

/* Function to return the nth term of A153343 */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *terms = NULL;   /* array of terms */
    static size_t terms_len = 0;  /* number of terms stored */
    static size_t terms_cap = 0;  /* allocated capacity */
    static idx_t next_k = 0;      /* next k to test */

    /* widen n to size_t for indexing */
    size_t target = (size_t)n;

    /* generate terms until we have at least target terms */
    while (terms_len < target) {
        /* test 5*next_k+4 for primality */
        idx_t val = 5 * next_k + 4;
        if (!is_prime(val)) {
            /* store the term */
            grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    /* return the requested term (1‑indexed) */
    return terms[target - 1];
}
