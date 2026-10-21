/*
 * A235708  Largest base in which n is pandigital.
 * https://oeis.org/A235708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = largest base b ≥ 2 such that the representation of n in
 *            base b uses all digits 0 … b‑1 (each digit at least once). If no
 *            such base exists, return 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute upper bound for base: L = floor(log2(n)) + 1.
       For base b, the number of digits in base b is ≤ L, so we only need to check b ≤ L. */
    unsigned L = 0;
    idx_t temp = n;
    while (temp) {
        temp >>= 1;
        L++;
    }
    if (L < 2)   /* n = 1 => no base ≥ 2 */
        return (seq_t)1;

    /* Numbers are tiny, so a small buffer is enough. */
    unsigned char used[64];   /* 64 is more than enough for any base ≤ 16 */
    for (unsigned b = L; b >= 2; b--) {
        /* reset flags */
        for (unsigned i = 0; i < b; i++) used[i] = 0;
        /* decompose n in base b and mark digits */
        idx_t t = n;
        while (t) {
            unsigned d = (unsigned)(t % b);
            used[d] = 1;
            t /= b;
        }
        /* check that all digits 0 .. b-1 appeared */
        bool ok = true;
        for (unsigned i = 0; i < b; i++) {
            if (!used[i]) { ok = false; break; }
        }
        if (ok) return (seq_t)b;
    }
    return (seq_t)1;
}
