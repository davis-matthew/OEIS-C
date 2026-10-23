/*
 * A138954  Number of complement symmetries in the rotations of the binary
 *          expansion of a number.
 * https://oeis.org/A138954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of rotations k of the binary expansion of n * such
 *            that rotate(n,k) equals the bitwise complement of n. A rotation
 *            * is a cyclic shift of the bits (most‑significant bit first).
 *            For each * k with 0 ≤ k < L (L = length of the binary string) we
 *            test whether * every bit x[i] in the orig...
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the special case n == 0: binary "0" has no complement
       rotation. */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* Determine the number of bits L of n. */
    idx_t temp = n;
    idx_t L = 0;
    while (temp > (idx_t)0) {
        temp >>= 1;
        ++L;
    }

    /* Store the bits from most significant to least significant. */
    unsigned int bits[64];   /* idx_t is uint64_t, so 64 bits suffice. */
    for (idx_t i = 0; i < L; ++i) {
        /* Extract the i-th bit (MSB first). */
        bits[i] = (unsigned int)((n >> (L - 1 - i)) & (idx_t)1);
    }

    seq_t count = 0;
    for (idx_t k = 0; k < L; ++k) {
        bool ok = true;
        for (idx_t i = 0; i < L; ++i) {
            if ((bits[i] + bits[(i + k) % L]) != 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            ++count;
        }
    }

    return count;
}
