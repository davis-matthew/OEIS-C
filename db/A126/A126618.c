/*
 * A126618  Simili-primes of order 3.
 * https://oeis.org/A126618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 5662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simili-primes of order 3: a(n) is the n-th integer such that it
   cannot be divided by any earlier such integer and it is the 3rd
   candidate found (after counting divisibility‑free numbers) in the
   natural number sequence.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------ */
/* Static state for memoisation and incremental generation.    */
/* ------------------------------------------------------------ */

static uint64_t *atoms = NULL;      /* discovered atoms */
static size_t atom_count = 0;       /* number of atoms stored    */
static size_t atom_cap = 0;         /* current array capacity    */
static uint64_t next_candidate = 2; /* next integer to examine   */
static idx_t candidate_seen = 0;    /* number of candidates seen */


/* Grow the atoms array when needed. */
static void ensure_atoms_capacity(void)
{
    if (atom_count >= atom_cap) {
        size_t new_cap = atom_cap ? atom_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(atoms, new_cap * sizeof(uint64_t));
        if (!tmp) abort();           /* allocation failure */
        atoms = tmp;
        atom_cap = new_cap;
    }
}

/* ------------------------------------------------------------ */
/* Sequence generator.                                          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 as a safety measure. */
        return (seq_t)0;
    }

    /* Initialise storage lazily. */
    if (!atoms) {
        atom_cap = 8;
        atoms = (uint64_t *)malloc(atom_cap * sizeof(uint64_t));
        if (!atoms) abort();          /* allocation failure */
    }

    /* Generate atoms until we have at least n of them. */
    while (atom_count < (size_t)n) {
        uint64_t x = next_candidate++;
        int divisible = 0;
        size_t i;

        for (i = 0; i < atom_count; ++i) {
            if (x % atoms[i] == 0) {
                divisible = 1;
                break;               /* divisible by an existing atom */
            }
        }

        if (!divisible) {
            candidate_seen++;
            if ((candidate_seen % 3) == 0) {
                /* New atom discovered. */
                ensure_atoms_capacity();
                atoms[atom_count++] = x;
            }
        }
    }

    return (seq_t)atoms[n - 1];
}
