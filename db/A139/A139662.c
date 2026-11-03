/*
 * A139662  Primes of the form x^2 + 462*y^2.
 * https://oeis.org/A139662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 376, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9694 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139662: primes of the form x^2 + 462*y^2.  
   A prime p is represented iff p mod 1848 is one of the 32
   allowed residues: 1,25,169,247,289,295,361,463,487,529,625,631,
   697,751,793,823,841,961,991,1087,1159,1255,1303,1345,1369,
   1423,1633,1681,1807,1831.
   The implementation scans successive primes, keeps those whose
   residue satisfies the condition, and memoises the found terms
   so subsequent calls are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
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
    /* Cache of discovered sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static uint64_t next_candidate = 2;          /* next integer to check for a prime */

    /* Predefined set of admissible residues modulo 1848 */
    static bool residue_ok[1848] = {0};
    static bool residues_initialized = false;
    if (!residues_initialized) {
        const unsigned int residues[] = {
            1, 25, 169, 247, 289, 295, 361, 463, 487, 529,
            625, 631, 697, 751, 793, 823, 841, 961, 991,
            1087, 1159, 1255, 1303, 1345, 1369, 1423,
            1633, 1681, 1807, 1831
        };
        const size_t rlen = sizeof(residues)/sizeof(residues[0]);
        for (size_t i = 0; i < rlen; ++i)
            residue_ok[residues[i]] = true;
        residues_initialized = true;
    }

    /* Extend cache until it contains at least n terms */
    while (cache_size < n) {
        uint64_t p = next_prime(next_candidate);
        next_candidate = p + 1;          /* next integer to start the search */

        if (residue_ok[p % 1848]) {
            /* Add to cache */
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
