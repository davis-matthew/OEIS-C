/*
 * A153153  Permutation of natural numbers: A059893-conjugate of A003188.
 * https://oeis.org/A153153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0 a(1) = 1 For n ≥ 2: Let w = floor(log₂ n), pow = 2^w
 *            (the largest power of two ≤ n) offset = n – pow If offset is
 *            even: k = offset / 2 a(n) = 2 * a(pow – 1 – k) + 1 else: k =
 *            (offset – 1) / 2 a(n) = 2 * a(pow/2 + k)
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* compute a(n) for the permutation A153153 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* find w = floor(log2(n)) ; n > 1 */
    unsigned int w = 63 - __builtin_clzll((uint64_t)n);
    uint64_t pow = 1ULL << w;               /* pow = 2^w */
    uint64_t offset = (uint64_t)n - pow;

    if ((offset & 1ULL) == 0) {             /* even offset -> lower part */
        uint64_t k = offset >> 1;           /* offset = 2*k */
        idx_t inner = (pow - 1ULL) - k;     /* n' = pow‑1‑k (< pow) */
        seq_t val = a(inner);
        return (val << 1) + 1;
    } else {                                /* odd offset -> upper part */
        uint64_t k = (offset - 1ULL) >> 1;  /* offset = 2*k+1 */
        idx_t inner = (pow >> 1) + k;       /* n' = pow/2 + k (< pow) */
        seq_t val = a(inner);
        return (val << 1);
    }
}
