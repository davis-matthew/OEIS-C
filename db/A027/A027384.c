/*
 * A027384  Number of distinct products i*j with 0 <= i, j <= n.
 * https://oeis.org/A027384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ i*j : 0 ≤ i, j ≤ n }|
 * Range:     u8 n <= 27, u16 n <= 502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct products of two integers in the range [0, n] */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Size of bitset: need bits for numbers 0..n*n inclusive */
    uint64_t max_prod = (uint64_t)n * (uint64_t)n;
    size_t bit_len   = (size_t)max_prod + 1;          /* number of required bits   */
    size_t words     = (bit_len + 63) >> 6;           /* number of 64‑bit words    */
    uint64_t *bits   = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (bits == NULL) return (seq_t)0; /* out of memory, return 0 */

    seq_t count = 0;

    for (idx_t i = 0; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            uint64_t prod = (uint64_t)i * (uint64_t)j;       /* product fits 64 bits for n ≤ 10 000 */
            size_t idx = (size_t)prod;
            size_t word = idx >> 6;
            uint64_t mask = ((uint64_t)1) << (idx & 63);
            if (!(bits[word] & mask)) {
                bits[word] |= mask;
                ++count;
            }
        }
    }

    free(bits);
    return (seq_t)count;
}
