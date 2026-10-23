/*
 * A010432  Squares mod 71.
 * https://oeis.org/A010432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth smallest quadratic residue modulo 71 (i.e. the
 *            sorted unique values of i^2 mod 71 for i ≥ 0, n = 1…36)
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static table of the 36 sorted quadratic residues modulo 71 */
    static bool initialized = false;
    static seq_t residues[36];
    static size_t count = 0;

    if (!initialized) {
        bool has[71] = {false};
        for (uint64_t i = 0; i < 71; ++i) {
            uint64_t r = (i * i) % 71;
            has[(size_t)r] = true;
        }

        size_t idx = 0;
        for (size_t r = 0; r < 71; ++r) {
            if (has[r]) {
                residues[idx++] = (seq_t)r;
            }
        }
        count = idx;          /* should be 36 for a prime 71 */
        initialized = true;
    }

    if (n >= 1 && (size_t)(n - 1) < count) {
        return residues[n - 1];
    }
    return (seq_t)0;          /* undefined for n > 36 */
}
