/*
 * A010436  Squares mod 75.
 * https://oeis.org/A010436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest quadratic residue modulo 75 (i.e. the
 *            integers x in {0,…,74} such that ∃k with k^2 ≡ x (mod 75))
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h> /* size_t */

seq_t a(idx_t n)
{
    static seq_t residues[75];
    static size_t count = 0;
    static bool init = false;

    if (!init) {
        bool used[75] = {0};
        size_t idx = 0;
        for (idx_t i = 0; i < (idx_t)75; ++i) {
            seq_t r = (seq_t)i * (seq_t)i % (seq_t)75;
            size_t ri = (size_t)r;
            if (!used[ri]) {
                used[ri] = true;
                residues[count++] = r;
            }
        }
        /* sort ascending (count ≤ 75) */
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (residues[i] > residues[j]) {
                    seq_t tmp = residues[i];
                    residues[i] = residues[j];
                    residues[j] = tmp;
                }
            }
        }
        init = true;
    }

    if (n == 0) return (seq_t)0;           /* invalid index for safety */
    if (n <= count) {
        return residues[(size_t)(n - 1)];
    }
    return (seq_t)0;                         /* undefined for n > count */
}
