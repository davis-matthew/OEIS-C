/*
 * A010396  Squares mod 35.
 * https://oeis.org/A010396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th element of the sorted set of quadratic residues
 *            modulo 35. The set of residues is
 *            {0,1,4,9,11,14,15,16,21,25,29,30}. The sequence repeats with
 *            period 12
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t residues[12];
    static bool initialized = false;
    if (!initialized) {
        const seq_t MOD = 35;
        bool seen[35] = {0};
        seq_t x;
        for (x = 0; x < MOD; ++x) {
            seq_t r = (x * x) % MOD;
            seen[(int)r] = true;
        }
        int k = 0;
        int r;
        for (r = 0; r < 35; ++r) {
            if (seen[r]) {
                residues[k++] = r;
                if (k == 12)
                    break;
            }
        }
        initialized = true;
    }
    idx_t idx = (n - 1) % 12;
    return residues[idx];
}
