/*
 * A107188  Primes of the form 6x^2 + 13y^2.
 * https://oeis.org/A107188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9602 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes of the form 6*x^2 + 13*y^2.
   The primes are exactly those congruent to
   {13, 19, 37, 67, 85, 109, 115, 163, 187,
    229, 253, 301, 307} modulo 312.
   The function builds the list on-demand and
   memoises them, so arbitrary calling order is allowed. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* enlarge array to store seq_t values */
static void add_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val) {
    if (*cap == 0) {
        *cap  = 4;
        *arr  = mem_new(seq_t, (*cap));
    }
    if (*size == *cap) {
        *cap <<= 1;
        *arr  = mem_resize(seq_t, *arr, (*cap));
    }
    (*arr)[*size] = val;
    (*size)++;
}

static seq_t *form_primes = NULL;  /* dynamically resized array of primes */
static size_t  form_cap   = 0;     /* current allocated capacity       */
static size_t  form_size  = 0;     /* number of primes stored          */
static uint64_t next_candidate = 2;/* next integer to test             */

/* The allowed residues modulo 312. */
static const uint16_t residues[13] = {
    13, 19, 37, 67, 85, 109, 115, 163, 187,
    229, 253, 301, 307
};

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; n == 0 is never requested,
       but we guard against it for robustness. */
    if (n == 0) return 0;

    /* Extend the list until we have at least n desired primes. */
    while (form_size < n) {
        /* Search for the next candidate that is prime and has an
           allowed residue modulo 312. */
        while (true) {
            if (is_prime_u64(next_candidate)) {
                uint64_t r = next_candidate % 312U;
                bool ok = false;
                for (int i = 0; i < 13; ++i) {
                    if (r == residues[i]) { ok = true; break; }
                }
                if (ok) {
                    /* Store the prime and prepare the next candidate. */
                    add_seq(&form_primes, &form_cap, &form_size,
                            (seq_t)next_candidate);
                    next_candidate += 2; /* skip to next odd number */
                    break;              /* exit inner search loop */
                }
            }
            /* Advance to the next odd candidate. */
            if (next_candidate == 2) {
                next_candidate = 3;
            } else {
                next_candidate += 2;
            }
        }
    }

    return form_primes[n - 1];  /* a(1) is at index 0 */
}
