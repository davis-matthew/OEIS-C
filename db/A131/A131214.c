/*
 * A131214  Fixed point of the morphism 1 -> 1357, 2 -> 1246, 3 -> 2357, 4 ->
 *          1346, 5 -> 2457, 6 -> 1356, 7 -> 2467.
 * https://oeis.org/A131214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth symbol in the fixed point of the morphism 1 → 1357, 2
 *            → 1246, 3 → 2357, 4 → 1346, 5 → 2457, 6 → 1356, 7 → 2467 The
 *            algorithm interprets n‑1 in base‑4 and walks the substitution
 *            tree
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* mapping table: sigma[x-1][r] gives the r‑th digit (0‑based) of the image of x */
    static const uint8_t sigma[7][4] = {
        {1,3,5,7}, /* 1 → 1357 */
        {1,2,4,6}, /* 2 → 1246 */
        {2,3,5,7}, /* 3 → 2357 */
        {1,3,4,6}, /* 4 → 1346 */
        {2,4,5,7}, /* 5 → 2457 */
        {1,3,5,6}, /* 6 → 1356 */
        {2,4,6,7}  /* 7 → 2467 */
    };

    /* Convert n-1 to base‑4 digits (least significant first) */
    idx_t x = n - 1;
    uint8_t digits[64];
    size_t len = 0;
    while (x) {
        digits[len++] = (uint8_t)(x & 3U); /* x % 4 */
        x >>= 2;                          /* x = x / 4 */
    }

    /* Walk through the digits from most significant to least, starting at root symbol 1 */
    uint8_t cur = 1;
    for (size_t i = len; i > 0; --i) {
        cur = sigma[cur - 1][digits[i - 1]];
    }

    return (seq_t)cur;
}
