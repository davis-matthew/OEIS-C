/*
 * A133884  a(n) = binomial(n+4,n) mod 4.
 * https://oeis.org/A133884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4) mod 4
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Periodicity length is 16 as described in OEIS.  Compute once. */
    static bool init = false;
    static seq_t period[16];

    if (!init) {
        for (idx_t i = 0; i < 16; ++i) {
            /* Compute binomial(i+4,4) exactly using a 64‑bit intermediate. */
            unsigned long long v = (unsigned long long)(i + 4) *
                                   (unsigned long long)(i + 3) *
                                   (unsigned long long)(i + 2) *
                                   (unsigned long long)(i + 1) / 24ULL;
            period[i] = (seq_t)(v % 4ULL);
        }
        init = true;
    }

    /* Return the value corresponding to n mod 16. */
    return period[(size_t)(n % 16)];
}
