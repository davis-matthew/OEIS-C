/*
 * A010430  Squares mod 69.
 * https://oeis.org/A010430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest quadratic residue modulo 69 (offset 1)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MOD 69
#define RES_COUNT 24

seq_t a(idx_t n)
{
    static seq_t residues[RES_COUNT];
    static bool initialized = false;
    static bool seen[MOD];

    if (!initialized) {
        /* mark all residues as not seen */
        for (int i = 0; i < MOD; ++i)
            seen[i] = false;

        /* compute squares mod MOD and record observed residues */
        for (int i = 0; i < MOD; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i % (seq_t)MOD;
            int r = (int)sq;            /* sq < MOD, safe cast to int */
            seen[r] = true;
        }

        /* collect the residues in ascending order */
        size_t idx = 0;
        for (int r = 0; r < MOD; ++r) {
            if (seen[r]) {
                residues[idx++] = (seq_t)r;
            }
        }
        /* idx should be RES_COUNT (24) */
        (void)idx; /* silence unused variable warning */
        initialized = true;
    }

    /* sequence is periodic with period RES_COUNT; return the appropriate term */
    return residues[(size_t)(n - 1) % RES_COUNT];
}
