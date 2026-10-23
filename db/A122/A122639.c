/*
 * A122639  n_{2n+1}.
 * https://oeis.org/A122639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the decimal digits of n in base (2n+1)
 * Range:     u8 n <= 39, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, if n = d_k … d_1 d_0 in decimal, then            */
/* a(n) = d_k·(2n+1)^k + … + d_1·(2n+1) + d_0. */

/* This implementation only uses integer arithmetic and works for all
   builds where seq_t can hold the required result. */

#include <stddef.h>          /* for size_t */

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least‑significant first). */
    uint8_t digits[20];      /* n≤2^64−1 ⇒ at most 20 decimal digits */
    size_t pos = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[pos++] = (uint8_t)(tmp % 10U);
        tmp /= 10U;
    }

    /* Base is 2n+1, represented in seq_t. */
    seq_t base = ((seq_t)n) * 2 + 1;

    seq_t result = 0;
    /* Process digits from most‑significant to least‑significant. */
    for (size_t i = pos; i > 0; --i) {
        result = result * base + (seq_t)digits[i - 1];
    }

    return result;
}
