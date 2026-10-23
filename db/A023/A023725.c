/*
 * A023725  Numbers with no 1's in their base-5 expansion.
 * https://oeis.org/A023725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = interpret (n‑1) from base‑4 as a base‑5 number where
 *            digits 0→0, 1→2, 2→3, 3→4. This produces all integers whose
 *            base‑5 expansion contains no digit 1, in increasing order
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification.  Convert n‑1 to base‑4 digits. */
    idx_t k = n - 1;
    /* 64 entries are plenty for 64‑bit n; each is a base‑4 digit (0‑3). */
    uint8_t digits[64];
    size_t len = 0;
    while (k > 0) {
        digits[len++] = (uint8_t)(k & 3ULL);   /* k % 4 */
        k >>= 2;                              /* k /= 4 */
    }

    /* Build the base‑5 result from the most significant digit. */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d4 = digits[len - 1 - i];
        seq_t d5 = (d4 == 0) ? 0
            : (d4 == 1) ? 2
            : (d4 == 2) ? 3
            : 4;                          /* d4 == 3 */
        res = res * (seq_t)5 + d5;
    }

    return res;  /* n == 1 ⇒ len == 0 ⇒ res == 0 as required */
}
