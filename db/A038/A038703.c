/*
 * A038703  Primes p such that p^2 mod q is odd, where q is the previous
 *          prime.
 * https://oeis.org/A038703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A038703: primes p such that p^2 mod q is odd, q being the previous prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
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

static seq_t *seq_cache = NULL;
static size_t seq_len = 0, seq_cap = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset is 1, undefined for n=0 */

    /* Initialise state on first call */
    static idx_t candidate = 3;    /* smallest odd prime to test */
    static idx_t last_prime = 2;   /* previous prime (starts with 2) */

    if (!initialized) {
        candidate = 3;
        last_prime = 2;
        initialized = true;
    }

    /* Generate terms until we have at least n entries */
    while (seq_len < n) {
        if (is_prime(candidate)) {
            /* p^2 mod q is odd? */
            seq_t sq_mod = powmod((seq_t)candidate, 2, (seq_t)last_prime);
            if ((sq_mod & 1) != 0) {
                append_candidate(&seq_cache, &seq_len, &seq_cap,
                                 (seq_t)candidate);
            }
            /* Update previous prime for the next investigation */
            last_prime = candidate;
        }
        candidate += 2;  /* skip even numbers */
    }

    return seq_cache[n - 1];
}
