/*
 * A010385  Squares mod 23.
 * https://oeis.org/A010385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th smallest quadratic residue modulo 23
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t residues[12];
static size_t residue_count = 0;
static bool initialized = false;

static void init_residues(void)
{
    bool used[23] = { false };
    seq_t v;
    for (idx_t i = 0; i < 23; ++i)
    {
        v = (seq_t)i * (seq_t)i % 23;
        used[(int)v] = true;   /* v < 23, fits in int */
    }
    size_t idx = 0;
    for (int j = 0; j < 23; ++j)
        if (used[j])
            residues[idx++] = (seq_t)j;
    residue_count = idx;
    /* Simple selection sort (residue_count == 12) */
    for (size_t i = 0; i < residue_count; ++i)
    {
        size_t min = i;
        for (size_t j = i + 1; j < residue_count; ++j)
            if (residues[j] < residues[min])
                min = j;
        if (min != i)
        {
            seq_t tmp = residues[i];
            residues[i] = residues[min];
            residues[min] = tmp;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_residues();
    /* if n is larger than the finite list, just cycle */
    return residues[(n - 1) % residue_count];
}
