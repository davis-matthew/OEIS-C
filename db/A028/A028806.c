/*
 * A028806  Nonsquares mod 93.
 * https://oeis.org/A028806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th positive integer that is not a quadratic residue
 *            modulo 93
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for offset 0, return 0 as a placeholder */

    static unsigned char is_square[93] = {0};
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i < 93; ++i) is_square[i] = 0;
        for (idx_t i = 0; i < 93; ++i) {
            idx_t r = (i * i) % 93;
            is_square[r] = 1;             /* mark quadratic residues modulo 93 */
        }
        initialized = true;
    }

    idx_t count = 0;
    idx_t cand   = 1;

    while (count < n) {                   /* iterate positives until nth non‑residue is found */
        if (!is_square[cand % 93]) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }

    return (seq_t)cand; /* should never be reached */
}
