/*
 * A211097  Number of factors in Lyndon factorization of binary vectors of
 *          lengths 1, 2, 3, ...
 * https://oeis.org/A211097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: number of factors in the Lyndon factorization
   of the binary word that is the n‑th element in the
   enumeration 0,1,00,01,10,11,000,...  */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* find length L of the binary word */
    size_t len = 0;
    size_t cumul = 0;
    while (1) {
        ++len;
        size_t next = cumul + ((size_t)1 << len);
        if ((size_t)n <= next) break;
        cumul = next;
    }

    /* index within words of this length (0‑based) */
    size_t idx = (size_t)n - cumul - 1;

    /* generate the word as a char array of '0'/'1' */
    static char w[64];                  /* 64 > max length for n≈1e4 */
    for (size_t i = 0; i < len; ++i) {
        size_t bit = (idx >> (len - 1 - i)) & 1;
        w[i] = bit ? '1' : '0';
    }

    /* Duval's algorithm to count Lyndon factors */
    size_t i = 0;
    size_t count = 0;
    while (i < len) {
        size_t j = i + 1, k = i;
        while (j < len && w[k] <= w[j]) {
            if (w[k] < w[j]) k = i;
            else ++k;
            ++j;
        }
        size_t l = j - k;
        while (i <= k) {
            i += l;
            ++count;
        }
    }

    return (seq_t)count;
}
