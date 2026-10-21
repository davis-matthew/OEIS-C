/*
 * A056991  Numbers with digital root 1, 4, 7 or 9.
 * https://oeis.org/A056991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = a(n-4) + 9 (n>4) with a(1)=1, a(2)=4, a(3)=7, a(4)=9.
 *            Equivalent closed form: a(n) = base[(n-1)%4] + 9*floor((n-1)/4)
 * Range:     u8 n <= 113, u16 n <= 9619, u32 n <= 9619, u64 n <= 9619,
 *            u128 n <= 9619, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t base[4] = {1, 4, 7, 9};

    if (n <= 4)                 /* offset 1 → indices 1‑4 */
        return base[n-1];

    idx_t k = (n - 1) / 4;      /* number of 4‑step jumps */
    idx_t m = (n - 1) % 4;      /* base index (0‑based) */

    seq_t result = base[m];
    seq_t add    = (seq_t)k * (seq_t)9;
    return result + add;
}
