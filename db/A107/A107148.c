/*
 * A107148  Primes of the form 2x^2 + 21y^2.
 * https://oeis.org/A107148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 827, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 2x^2 + 21y^2;
   primes are exactly those with p % 168 ∈ {2,23,29,53,71,95,149} */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_uint64.h"

#define NUM_RESIDUES 7
static const unsigned int residues[NUM_RESIDUES] = {2,23,29,53,71,95,149};

seq_t a(idx_t n)
{
    /* static storage for memoisation and initialization */
    static bool initialized = false;
    static seq_t *seq = NULL;      /* 1‑based array of found primes */
    static size_t seq_size = 0;    /* number of primes stored */
    static size_t seq_cap  = 0;    /* capacity of seq array */
    static bool allowed[168] = {0};/* mask of admissible residues mod 168 */
    static uint64_t last_checked = 0; /* last integer tested */

    /* one‑time setup: build residue mask and first array allocation */
    if (!initialized) {
        for (int i = 0; i < NUM_RESIDUES; ++i)
            allowed[residues[i]] = true;

        seq_cap = 16;                    /* initial capacity */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;                      /* 1‑based indexing; index 0 unused */
        seq_size = 0;
        last_checked = 0;
        initialized = true;
    }

    /* grow the sequence until we have at least n terms */
    while (seq_size < n) {
        /* move to the next candidate integer */
        if (last_checked == 0) {
            last_checked = 2;                /* start at 2 */
        } else if (last_checked == 2) {
            last_checked = 3;                /* next odd */
        } else {
            last_checked += 2;                /* skip evens */
        }

        /* primality test */
        bool prime_flag = (last_checked == 2) ? true
                                            : is_prime_uint64(last_checked);

        /* accept the prime if it matches the required residues */
        if (prime_flag &&
            (last_checked == 2 || allowed[(int)(last_checked % 168)])) {

            /* grow array if necessary */
            if (seq_size + 1 >= seq_cap) {
                seq_cap <<= 1; /* double the capacity */
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[++seq_size] = (seq_t)last_checked;
        }
    }

    return seq[n];
}
