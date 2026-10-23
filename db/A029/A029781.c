/*
 * A029781  Squares such that digits of sqrt(n) appear in both n and n^(3/2).
 * https://oeis.org/A029781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A029780(n)^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   get_digits, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS sequence A029781.  It
   generates successive integers k, tests whether each digit of
   k appears in k^2 and in k^3, and when the (n‑th) qualifying
   number is found, returns its square k^2.  The algorithm is
   O(k) for k up to the n‑th valid entry. */

#include <stdint.h>
#include <stddef.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/seq_mul.h"

/* Returns a bit mask of the decimal digits present in x */
static inline uint16_t digit_mask(seq_t x)
{
    uint16_t mask = 0;
    size_t len = 0;
    uint8_t buf[128];                     /* ample buffer for digits */
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i)
        mask |= (uint16_t)(1U << buf[i]);
    return mask;
}

seq_t a(idx_t n)
{
    /* n is assumed >= 1 */
    idx_t found = 0;

    for (idx_t i = 0; ; ++i) {
        seq_t k     = (seq_t)i;
        uint16_t km = digit_mask(k);

        seq_t k_sq  = seq_mul(k, k);
        uint16_t sqm = digit_mask(k_sq);
        if ((km & ~sqm) != 0)          /* some digit of k missing in k^2 */
            continue;

        seq_t k_cu  = seq_mul(k_sq, k);
        uint16_t cum = digit_mask(k_cu);
        if ((km & ~cum) != 0)          /* some digit of k missing in k^3 */
            continue;

        /* k satisfies the property */
        ++found;
        if (found == n)
            return k_sq;              /* a(n) = k^2 */
    }
}
