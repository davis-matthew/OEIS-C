/*
 * A177686  If a1a2a3 is a 3-digit integer in a concatenated form, we define
 *          two permutations of its digits as follows: P1(a1a2a3)=a2a3a1 and
 *          P2(a1a2a3)=a1a3a2, then we take the absolute value of their
 *          difference. Thus we form a sequence: a1a2a3,
 *          abs(P1(a1a2a3)-P2(a1a2a3)), and so on.
 * https://oeis.org/A177686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   a(n) = [99, 891, 198, 792, 297, 693, 396, 594, 495][(n-1) mod 9]
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Index starting at 1; wrap around after 9 terms */
    static const seq_t values[9] = {
        99, 891, 198, 792, 297, 693, 396, 594, 495
    };
    /* Compute zero‑based position in the cycle */
    idx_t pos = (n - 1) % 9;
    return values[pos];
}
