/*
 * A187127  Triangular numbers k*(k+1)/2 mod 100, sorted and uniqued.
 * https://oeis.org/A187127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = the nth smallest distinct residue of k*(k+1)/2 mod 100 (k
 *            ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t seq[44];

static void compute_sequence(void)
{
    bool seen[100] = {0};
    for (idx_t k = 0; k < 200; ++k) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;
        uint8_t r = (uint8_t)(t % (seq_t)100);
        seen[r] = true;
    }
    size_t idx = 0;
    for (int r = 0; r < 100; ++r)
        if (seen[r])
            seq[idx++] = (seq_t)r;   /* already sorted by increasing r */
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) compute_sequence();
    if (n == 0) return 0;                     /* undefined offset < 1, return 0 */
    if (n > 44) return 0;                     /* sequence has only 44 terms */
    return seq[n - 1];
}
