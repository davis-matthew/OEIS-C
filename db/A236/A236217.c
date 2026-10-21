/*
 * A236217  Numbers not divisible by 3, 5 or 11.
 * https://oeis.org/A236217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers not divisible by 3, 5 or 11. This sequence is periodic
 *            modulo 165. The 80 admissible residues repeat every 165. a(n) =
 *            165 * floor((n-1)/80) + r[(n-1)%80]
 * Range:     u8 n <= 123, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The residues r[k] are numerically 1,2,4,7,8,13,14,16,17,19,23,26,28,29,
   31,32,34,37,38,41,43,46,47,49, ...  (80 values total). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t residues[80];

static void init_residues(void)
{
    idx_t cnt = 0;
    for (idx_t x = 1; x <= 165 && cnt < 80; ++x)
    {
        if (x % 3 != 0 && x % 5 != 0 && x % 11 != 0)
        {
            residues[cnt++] = (seq_t)x;
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        init_residues();
        initialized = true;
    }

    /* n is guaranteed to be >= 1 by the OEIS offset. */
    idx_t n_minus_one = n - 1;
    idx_t block_idx  = n_minus_one / 80;      /* 0‑based block number */
    idx_t offset_idx = n_minus_one % 80;      /* position in the block */

    seq_t block = (seq_t)block_idx;
    seq_t result = block * (seq_t)165 + residues[(size_t)offset_idx];
    return result;
}
