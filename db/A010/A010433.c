/*
 * A010433  Squares mod 72.
 * https://oeis.org/A010433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = distinct residues of m^2 mod 72, sorted ascending
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t residues[72];
    static size_t cnt = 0;
    static bool initialized = false;

    if (!initialized) {
        /* compute unique residues modulo 72 */
        size_t uniq_cnt = 0;
        for (idx_t i = 0; i < (idx_t)72; ++i) {
            idx_t r = (i * i) % (idx_t)72;
            bool found = false;
            for (size_t j = 0; j < uniq_cnt; ++j) {
                if (residues[j] == (seq_t)r) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                residues[uniq_cnt++] = (seq_t)r;
            }
        }
        /* sort the residues in ascending order */
        for (size_t i = 0; i < uniq_cnt; ++i) {
            for (size_t j = i + 1; j < uniq_cnt; ++j) {
                if (residues[j] < residues[i]) {
                    seq_t t = residues[i];
                    residues[i] = residues[j];
                    residues[j] = t;
                }
            }
        }
        cnt = uniq_cnt;
        initialized = true;
    }

    if (n > 0 && (idx_t)(n - 1) < cnt) {
        return residues[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
