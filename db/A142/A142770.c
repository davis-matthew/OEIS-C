/*
 * A142770  Primes congruent to 43 mod 59.
 * https://oeis.org/A142770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 43 modulo 59 (p % 59 == 43)
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 972 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *primes = NULL;   /* array of primes */
    static size_t cap     = 0;     /* allocated capacity */
    static idx_t  len     = 0;     /* number of primes stored */
    static idx_t  cand    = 43;    /* current candidate to test */

    if (n == 0) return 0;          /* undefined for 0, ignore */

    /* Generate primes until we have at least n of them */
    while (len < (size_t)n) {
        /* test the current candidate */
        if (is_prime(cand)) {
            /* ensure capacity for the new prime */
            if (len == cap)
                grow_seq(&primes, &cap, len + 1);
            primes[len++] = (seq_t)cand;
        }
        cand += 59;                /* move to the next candidate of same residue */
    }

    return primes[n - 1];
}
