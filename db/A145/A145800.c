/*
 * A145800  a(n) = the smallest positive integer that is an (odd) palindrome
 *          when represented in binary and that contains within it the binary
 *          representation of n.
 * https://oeis.org/A145800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest odd binary palindrome that contains the binary *
 *            representation of n as a contiguous substring
 * Range:     u8 n <= 33, u16 n <= 555, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   propagate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/propagate.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; return 0 for safety */
    if (n == 0) return 0;

    /* number of bits in n (no leading zeros) */
    idx_t m = 0;
    seq_t tmp = (seq_t)n;
    while (tmp != 0) {
        tmp >>= 1;
        ++m;
    }

    /* store bits of n, MSB at index 0 */
    unsigned char nbits[128];
    tmp = (seq_t)n;
    for (idx_t i = 0; i < m; ++i) {
        nbits[m - 1 - i] = (unsigned char)(tmp & 1);
        tmp >>= 1;
    }

    /* search increasing palindrome lengths; we bound the loop by 2*m+2
       which is always sufficient for a solution */
    for (idx_t L = m; L <= m * 2 + 2; ++L) {
        bool any_found = false;
        seq_t best = 0;
        idx_t maxpos = (L >= m) ? L - m : 0;

        for (idx_t pos = 0; pos <= maxpos; ++pos) {
            signed char bits[256];                /* enough for all L <= 256 */
            for (idx_t i = 0; i < L; ++i) bits[i] = -1;

            /* place binary representation of n at position pos */
            for (idx_t i = 0; i < m; ++i) bits[pos + i] = (signed char)nbits[i];

            /* propagate symmetry constraints */
            if (!propagate(bits, L)) continue;

            /* enforce leading 1 (palindrome must start and end with 1) */
            if (bits[0] == -1) {
                bits[0] = bits[L - 1] = 1;
            } else if (bits[0] == 0) {
                continue;          /* conflict with required leading 1 */
            } else {
                if (bits[L - 1] == -1) bits[L - 1] = 1;
                else if (bits[L - 1] == 0) continue;  /* conflict after propagation */
            }

            /* final propagation after setting the leading 1 */
            if (!propagate(bits, L)) continue;

            /* set any remaining unset bits to 0 to minimise the value */
            for (idx_t i = 0; i < L; ++i)
                if (bits[i] == -1) bits[i] = 0;

            /* compute integer value of the palindrome */
            seq_t val = 0;
            for (idx_t i = 0; i < L; ++i) {
                val = (val << 1) | (seq_t)bits[i];
            }

            if (!any_found || val < best) {
                best = val;
                any_found = true;
            }
        }

        if (any_found) return best;          /* minimal value for this length */
    }

    return 0;   /* should never be reached for valid n */
}
