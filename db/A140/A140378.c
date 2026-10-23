/*
 * A140378  Lengths of runs of consecutive primes and nonprimes in A007775.
 * https://oeis.org/A140378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140378: lengths of runs of primes and composites in the
 * sequence of integers coprime to 30 (A007775). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Return the n‑th (1‑based) term of A140378. */
seq_t a(idx_t n)
{
    /* undefined calls with n == 0 do not occur; return 0 safely. */
    if (n == 0) return 0;

    /* Static state to keep the cache and scanning position between
    ** calls.  The cache contains all completed run lengths. */
    static seq_t   *runs     = NULL;
    static size_t  runs_cap = 0;
    static size_t  runs_len = 0;

    /* Scanning state for the current run. */
    static uint64_t cur      = 1;   /* next integer to examine */
    static bool     in_run   = false;
    static bool     cur_prime = false;
    static seq_t   cur_len    = 0;

    /* Coprime residues modulo 30: true iff residue ∈ {1,7,11,13,17,19,23,29}. */
    static const bool coprime[30] = {
        false, true, false, false, false, false, false, true,
        false, false, false, true, false, true, false, false,
        false, true, false, true, false, false, false, true,
        false, false, false, false, false, true
    };

    /* Extend the cache until it contains at least n run lengths. */
    while (runs_len < (size_t)n) {
        /* Skip integers that are not coprime to 30. */
        while (!coprime[cur % 30]) ++cur;

        bool prime = is_prime(cur);

        if (!in_run) {
            /* Start the very first run. */
            in_run   = true;
            cur_prime = prime;
            cur_len   = 1;
        } else if (prime == cur_prime) {
            /* Same type – extend the current run. */
            ++cur_len;
        } else {
            /* Current run ends. Store its length. */
            if (runs_len == runs_cap) {
                size_t new_cap = runs_cap ? runs_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failure */
                runs = tmp;
                runs_cap = new_cap;
            }
            runs[runs_len++] = cur_len;

            /* Start the next run with the current number. */
            cur_prime = prime;
            cur_len   = 1;
        }

        ++cur;   /* move to the next integer for the next iteration */
    }

    /* Return the requested run length. */
    return runs[(size_t)n - 1];
}
