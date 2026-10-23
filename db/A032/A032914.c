/*
 * A032914  Numbers whose set of base-7 digits is {1,3}.
 * https://oeis.org/A032914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose set of base‑7 digits is {1,3}. */
/* For an index n≥1, let L = floor(log2(n+1)).  Then the
   nth term is obtained by interpreting the binary representation
   of offset = n-(2^L-1) (with L bits) as a base‑7 number with
   digit 1 for bit 0 and digit 3 for bit 1. */

#include <stdint.h>

static inline idx_t floor_log2(idx_t v)
{
    idx_t log = 0;
    while (v >>= 1)
        ++log;
    return log;
}

seq_t a(idx_t n)
{
    /* Compute the length of the base‑7 representation */
    idx_t L = floor_log2(n + 1);          /* L = ⌊log2(n+1)⌋ */

    /* Compute 2^L */
    idx_t pow2 = ((idx_t)1) << L;

    /* Offset within the block of numbers having length L */
    idx_t offset = n - (pow2 - 1);        /* zero‑based offset, 0 ≤ offset < 2^L */

    /* Convert the binary offset to a base‑7 number using digits 1/3 */
    seq_t result = 0;
    for (idx_t i = L; i > 0; --i) {
        /* Extract the (i‑1)th bit (most significant first) */
        int bit = (offset >> (i - 1)) & 1;
        result = result * 7 + (seq_t)(bit ? 3 : 1);
    }
    return result;
}
