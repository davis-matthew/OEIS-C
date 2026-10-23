/*
 * A184986  Quintic residues modulo 3331, sorted.
 * https://oeis.org/A184986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) returns the n-th smallest residue of k⁵ modulo 3331
 *            (sorted)
 * Range:     u8 n <= 3, u16 n <= 667, u32 n <= 667, u64 n <= 667,
 *            u128 n <= 667, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"   /* for mod_pow(seq_t, uint64_t, seq_t) */

static seq_t *residues = NULL;  /* sorted array of distinct residues */
static size_t residue_count = 0;

/* Precompute all quintic residues modulo 3331 and store them sorted. */
static void init_residues(void)
{
    if (residues != NULL)
        return;                         /* already initialized */

    /* Temporary boolean array to mark present residues */
    static bool seen[3331] = {0};

    /* Compute residues for k = 0 .. 3330 */
    const seq_t MOD = (seq_t)3331;
    for (idx_t k = 0; k < 3331; ++k) {
        seq_t r = mod_pow((seq_t)k, (uint64_t)5, MOD);
        seen[(size_t)r] = true;
    }

    /* Count distinct residues (should be 667) */
    for (idx_t r = 0; r < 3331; ++r)
        if (seen[(size_t)r])
            ++residue_count;

    /* Allocate and populate the sorted residue array */
    residues = (seq_t *)malloc(residue_count * sizeof(seq_t));
    if (!residues)
        return; /* allocation failure: we silently fall back to empty array */

    idx_t idx = 0;
    for (idx_t r = 0; r < 3331; ++r) {
        if (seen[(size_t)r]) {
            residues[idx++] = (seq_t)r;
            if (idx == residue_count)
                break;
        }
    }
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)           /* invalid index – return 0 as a safe value */
        return (seq_t)0;

    init_residues();

    /* n is 1‑based; convert to 0‑based for array access */
    idx_t i = n - 1;
    if (i < residue_count)
        return residues[i];
    else
        return (seq_t)0;  /* out‑of‑range: no more residues */
}
