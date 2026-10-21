/*
 * A061786  Number of distinct sums i^2 + j^2 for 1<=i<=j<=n.
 * https://oeis.org/A061786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of distinct sums i^2 + j^2 with 1 ≤ i ≤ j ≤ n
 * Range:     u8 n <= 24, u16 n <= 454, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses a bitset to mark reachable sums and counts the bits. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* precompute number of 1‑bits in a byte */
static unsigned char bits_in_byte[256];
static int bits_in_byte_init = 0;

static void init_bits_in_byte(void)
{
    if (!bits_in_byte_init) {
        for (int k = 0; k < 256; ++k) {
            unsigned char x = (unsigned char)k;
            unsigned char cnt = 0;
            for (int d = 0; d < 8; ++d) {
                cnt += x & 1;
                x >>= 1;
            }
            bits_in_byte[k] = cnt;
        }
        bits_in_byte_init = 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but safe return */

    /* Maximum attainable sum is 2*n*n.  Map each sum to one bit. */
    idx_t max_sum = 2 * n * n;
    size_t byte_len = (size_t)((max_sum >> 3) + 1);

    uint8_t *bitmap = (uint8_t *)calloc(byte_len, 1);
    if (!bitmap)
        return 0;   /* allocation failed */

    /* Precompute squares as 64‑bit values (i^2 <= 1e8 for n<=10000). */
    uint64_t *sq = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!sq) {
        free(bitmap);
        return 0;
    }
    for (idx_t i = 1; i <= n; ++i)
        sq[i] = (uint64_t)i * (uint64_t)i;

    init_bits_in_byte();

    /* Mark all attainable sums */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t sumi = sq[i];
        for (idx_t j = i; j <= n; ++j) {
            uint64_t sum64 = sumi + sq[j];
            size_t byte_idx = (size_t)(sum64 >> 3);
            uint8_t bit = 1u << (uint8_t)(sum64 & 7u);
            bitmap[byte_idx] |= bit;
        }
    }

    /* Count set bits */
    uint64_t count64 = 0;
    for (size_t k = 0; k < byte_len; ++k)
        count64 += bits_in_byte[bitmap[k]];

    free(bitmap);
    free(sq);

    return (seq_t)count64;
}
