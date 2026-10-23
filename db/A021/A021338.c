/*
 * A021338  Decimal expansion of 1/334.
 * https://oeis.org/A021338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit n of 1/334 (a(0) is the first digit after
 *            the point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by long division: start with remainder r = 1; for each i = 0..n
 * compute digit = ⌊(r*10)/334⌋ and update r = (r*10) mod 334. */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t remainder = 1ULL;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t prod = remainder * 10ULL;       /* < 3340, fits in 64‑bit */
        digit = (seq_t)(prod / 334ULL);          /* the n‑th digit */
        remainder = prod % 334ULL;               /* new remainder */
    }
    return digit;
}
