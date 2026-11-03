/*
 * A235148  Primes p such that (p reversed) + 5 is also a prime.
 * https://oeis.org/A235148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 558, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 971 (2026-11-03)
 * Helpers:   is_prime, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235148: primes p such that rev(p)+5 is also prime. */
/* The function returns the n-th such prime (a(1)=2). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n) {
    /* Check that the index is valid (>=1).  Behaviour for n==0 is not required. */
    if (n == 0)
        return (seq_t)0;

    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* Keep track of how far we have searched in the prime sequence. */
    static idx_t last_checked = 1;

    /* Generate until we have at least 'n' terms. */
    while (seq_len < (size_t)n) {
        /* Find next prime number. */
        do {
            ++last_checked;
        } while (!is_prime(last_checked));

        /* Check condition: reversed prime + 5 is also prime. */
        idx_t revp5 = reverse_digits_idx(last_checked) + 5;
        if (is_prime(revp5)) {
            /* Append the found prime to the cache. */
            append_candidate(&seq, &seq_len, &seq_cap, (seq_t)last_checked);
        }
    }

    /* seq_len >= n now.  Return the requested element (1‑based index). */
    return seq[(size_t)(n - 1)];
}
