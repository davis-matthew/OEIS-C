/*
 * A028760  Nonsquares mod 47.
 * https://oeis.org/A028760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nonsquare residues modulo 47, repeated every 47 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* On first call compute all residues 1..46 that are not quadratic
       residues modulo 47. Uniquely determined because 47 is prime.   */
    static idx_t residues[23];
    static bool initialized = false;

    if (!initialized) {
        bool is_sq[47] = {0};          /* is_sq[x] = true iff x is a square mod 47 */
        for (idx_t i = 1; i < 47; ++i) {
            idx_t sq = (i * i) % 47;
            is_sq[sq] = true;
        }
        idx_t k = 0;
        for (idx_t x = 1; x < 47; ++x) {
            if (!is_sq[x]) {
                residues[k++] = x;
            }
        }
        initialized = true;
    }

    idx_t pos   = (n - 1) % 23;      /* 0‑based index within the residue list   */
    idx_t block = (n - 1) / 23;      /* number of complete 47‑cycles           */

    return (seq_t)47 * (seq_t)block + (seq_t)residues[pos];
}
