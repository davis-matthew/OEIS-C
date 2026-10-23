/*
 * A122646  phi(n)_n where phi() = A000010.
 * https://oeis.org/A122646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value of the decimal representation of φ(n) when
 *            interpreted as a base‑n numeral
 * Range:     u8 n <= 52, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t phi_val = phi(n);

    /* Extract decimal digits of φ(n) (least significant first) */
    unsigned char digits[256];      /* should be enough for any seq_t s.t. φ(n) < 10^256 */
    size_t pos = 0;
    seq_t temp = phi_val;

    /* Special case φ(n) == 0 – only happens for n = 0, but n ≥ 1 by definition.
       We handle it for completeness. */
    if (temp == 0) {
        digits[pos++] = 0;
    } else {
        while (temp != 0) {
            seq_t rem = temp % (seq_t)10;          /* 0 … 9 */
            digits[pos++] = (unsigned char)(long long)rem;
            temp = temp / (seq_t)10;
        }
    }

    /* Interpret the extracted digits as a base‑n number:
       result = d0*n^0 + d1*n^1 + … + d_{pos-1}*n^{pos-1}
       Equivalent to: result = 0; for i from pos‑1 down to 0: result = result*n + digits[i]   */
    seq_t result = 0;
    for (size_t i = pos; i > 0; --i) {
        result = result * (seq_t)n + (seq_t)digits[i - 1];
    }

    return result;
}
