/*
 * A088156  Value of n-th digit in vigesimal representation of n^n.
 * https://oeis.org/A088156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n^n / 20^n) mod 20
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the special case a(0) = 1 according to the OEIS entry */
    if (n == 0u)
        return (seq_t)1;

    /* We keep the base‑20 representation of n^k modulo 20^(n+1)
       while multiplying repeatedly by n.  This requires at most n+1
       digits, each in the range 0–19. */
    idx_t digits = n + 1u;                     /* number of stored digits   */
    uint8_t *d = (uint8_t *)malloc((size_t)digits);
    if (!d) return (seq_t)0;                  /* safety fallback (shouldn't happen) */

    /* Initialise to 1 (nothing else influences lower digits). */
    for (idx_t i = 0; i < digits; ++i) d[i] = 0;
    d[0] = 1;

    /* Perform n multiplications by the integer n.
       After each multiplication we keep only the lowest n+1 digits,
       hence effectively computing modulo 20^(n+1). */
    for (idx_t mul = 0; mul < n; ++mul) {
        uint32_t carry = 0;
        for (idx_t j = 0; j < digits; ++j) {
            uint32_t prod = (uint32_t)d[j] * (uint32_t)n + carry;
            d[j] = (uint8_t)(prod % 20u);
            carry = prod / 20u;
        }
        /* The carry beyond the n‑th digit would be discarded
           (i.e. modulo 20^(n+1)). */
    }

    seq_t result = (seq_t)d[n];   /* The n‑th digit of the base‑20 expansion */
    free(d);
    return result;
}
