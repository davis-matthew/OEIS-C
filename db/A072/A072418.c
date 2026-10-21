/*
 * A072418  Parity of floor(3^n/2^n).
 * https://oeis.org/A072418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (3/2)^n ) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return the parity of floor(3^n / 2^n).  The function works for
   n >= 1.  For n == 0 it returns 0 (undefined case).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* We need the n-th bit of 3^n; keep only the low (n+1) bits. */
    size_t bits_needed = n + 1;
    size_t words      = (bits_needed + 63u) / 64u;    /* number of 64‑bit words */

    uint64_t *w = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!w)
        return 0;  /* allocation failure – return 0 as a safe default */

    for (size_t i = 0; i < words; ++i)
        w[i] = 0;
    w[0] = 1;          /* start with 3^0 */

    /* Multiply by 3 n times, keeping only the low (n+1) bits. */
    for (idx_t step = 1; step <= n; ++step) {
        uint64_t carry = 0;
        for (size_t j = 0; j < words; ++j) {
            unsigned __int128 tmp = (unsigned __int128)w[j] * 3 + carry;
            w[j] = (uint64_t)tmp;
            carry = (uint64_t)(tmp >> 64);
        }
        /* mask the high bits that belong to positions >= n+1 */
        size_t bits_last = bits_needed & 63u;   /* remainder of bits_needed / 64 */
        if (bits_last) {
            uint64_t mask = ((uint64_t)1 << bits_last) - 1;
            w[words - 1] &= mask;
        }
    }

    /* Extract the n-th bit (0‑based) from the resulting value. */
    size_t word_index = n >> 6;    /* n / 64 */
    size_t bit_pos    = n & 63;    /* n % 64 */
    uint64_t bit = (w[word_index] >> bit_pos) & 1ULL;

    free(w);
    return (seq_t)bit;
}
