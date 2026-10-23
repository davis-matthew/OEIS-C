/*
 * A028802  Nonsquares mod 89.
 * https://oeis.org/A028802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th positive integer that is **not** a quadratic residue
 *            modulo 89
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool residues[89];
static bool residues_inited = false;

static void init_residues(void)
{
    for (idx_t i = 0; i < 89; ++i)
        residues[(unsigned)((i * i) % 89)] = true;
    residues_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (!residues_inited)
        init_residues();

    idx_t count = 0;
    idx_t x = 1;
    while (1) {
        if (!residues[(unsigned)(x % 89)]) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
}
