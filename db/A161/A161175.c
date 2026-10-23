/*
 * A161175  Triangle read by rows, modified Thue-Morse sequence (A010060 with
 *          offset 1): change 0 to 2, else 1.
 * https://oeis.org/A161175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 1 + parity(popcount(n - 2^floor(log2 n)))
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (1 for even popcount, 2 for odd popcount) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offsets start at 1 – if n is 0 just return 0 (unused). */
    if (n == 0)
        return 0;

    /* Find the highest power of two <= n (msb). */
    idx_t msb = 1;
    while (msb <= n) {
        /* Prevent overflow; stop if next shift would exceed n. */
        if (msb > n >> 1)
            break;
        msb <<= 1;
    }
    /* If the loop left msb > n (because msb doubled beyond n), shift back. */
    if (msb > n)
        msb >>= 1;

    /* Position within the current row. */
    idx_t j = n - msb;

    /* Compute parity of popcount(j). Toggle once per set bit. */
    bool odd = false;
    while (j) {
        odd = !odd;
        j &= j - 1;   /* drop lowest set bit */
    }

    /* 1 for even parity, 2 for odd parity. */
    return odd ? (seq_t)2 : (seq_t)1;
}
