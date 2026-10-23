/*
 * A138904  Number of rotational symmetries in the binary expansion of a
 *          number.
 * https://oeis.org/A138904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of rotational symmetries of the binary expansion
 *            of n. For n > 0, a(n) = L / d where L is the length of the
 *            binary representation (in bits) and d is its smallest period.
 *            For n == 0, a(0) = 1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0)
        return 1;

    /* Determine the binary length L and record bits MSB‑first */
    int L = 0;
    idx_t temp = n;
    while (temp) { temp >>= 1; ++L; }
    unsigned char bits[65];          /* L <= 64 for idx_t == uint64_t */
    for (int i = 0; i < L; ++i)
        bits[i] = (n >> (L - 1 - i)) & 1U;

    /* Find the smallest period d that divides L */
    for (int d = 1; d <= L; ++d) {
        if (L % d != 0)
            continue;
        bool ok = true;
        for (int i = 0; i < L; ++i) {
            if (bits[i] != bits[(i + d) % L]) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)(L / d);
    }

    /* Fallback: identity rotation only */
    return 1;
}
