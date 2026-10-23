/*
 * A033307  Decimal expansion of Champernowne constant (or Mahler's number),
 *          formed by concatenating the positive integers.
 * https://oeis.org/A033307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons, easy, changed
 * Formula:   a(n) = nth digit (0‑based) of Champernowne's constant in base 10
 * Range:     u8 n <= 9565, u16 n <= 9565, u32 n <= 9565, u64 n <= 9565,
 *            u128 n <= 9565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t L = 1;
    idx_t pow10 = 1; /* 10^(L-1) */

    /* Find the block (digit length) containing position n. */
    while (1) {
        idx_t count = 9 * pow10;               /* numbers with L digits   */
        unsigned __int128 seg = (unsigned __int128)count * L;
        if ((unsigned __int128)n < seg)
            break;
        n -= (idx_t)seg;
        ++L;
        pow10 *= 10;
    }

    idx_t offset = n;                 /* 0‑based offset within the L‑digit block */
    idx_t indexInBlock = offset / L;  /* which number in this block           */
    idx_t digitPos     = offset % L;  /* position in that number              */

    idx_t number = pow10 + indexInBlock;  /* the actual integer                */

    /* Extract the digit at position digitPos (most significant digit == 0). */
    idx_t divisor = pow10;
    for (idx_t i = 0; i < digitPos; ++i)
        divisor /= 10;

    return (seq_t)((number / divisor) % 10);
}
