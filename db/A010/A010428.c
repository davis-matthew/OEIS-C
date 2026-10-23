/*
 * A010428  Squares mod 67.
 * https://oeis.org/A010428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth smallest quadratic residue modulo 67. The
 *            sequence contains all distinct values k² mod 67 for k = 0..66,
 *            sorted
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Maximum number of distinct quadratic residues modulo a prime p is (p+1)/2.
   For p = 67, we have 34 such residues. */
#define MOD 67
#define RES_COUNT ((MOD + 1) / 2)

static seq_t residues[RES_COUNT];
static bool initialized = false;

/* Build the sorted list of residues on first use. */
static void init_residues(void)
{
    bool seen[MOD] = { false };
    size_t pos = 0;
    idx_t k;

    /* Mark all quadratic residues. */
    for (k = 0; k < MOD; ++k) {
        seq_t val = (seq_t)k * (seq_t)k;
        val = val % (seq_t)MOD;
        seen[(size_t)val] = true;
    }

    /* Collect them in ascending order. */
    for (size_t v = 0; v < MOD; ++v) {
        if (seen[v]) {
            residues[pos++] = (seq_t)v;
        }
    }
    /* pos should equal RES_COUNT but we don't rely on it. */
    initialized = true;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_residues();
    }
    if (n == 0 || n > (idx_t)RES_COUNT) {
        return (seq_t)0;  /* Undefined for n out of range. */
    }
    return residues[(size_t)(n - 1)];
}
