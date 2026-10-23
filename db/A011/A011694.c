/*
 * A011694  A binary m-sequence: expansion of reciprocal of x^8+x^7+x^5+x^3+1.
 * https://oeis.org/A011694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let b(k) be the coefficients of the series 1/(1 + x³ + x⁵ + x⁷ +
 *            x⁸) over GF(2): b(0) = 1, for k > 0 b(k) = b(k-3) ^ b(k-5) ^
 *            b(k-7) ^ b(k-8) (terms with negative indices are omitted). Then
 *            a(n) = 0 for n < 7 and a(n) = b(n‑7) for n ≥ 7. This recurrence
 *            produces a 255‑periodic binary sequence
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool inited = false;
    static seq_t tbl[255];          /* b(0) … b(254) */

    if (!inited) {
        tbl[0] = 1;
        for (size_t k = 1; k < 255; ++k) {
            seq_t v = 0;
            if (k >= 3) v ^= tbl[k-3];
            if (k >= 5) v ^= tbl[k-5];
            if (k >= 7) v ^= tbl[k-7];
            if (k >= 8) v ^= tbl[k-8];
            tbl[k] = v;
        }
        inited = true;
    }

    if (n < 7)
        return 0;

    size_t pos = (size_t)((n - 7) % 255);
    return tbl[pos];
}
