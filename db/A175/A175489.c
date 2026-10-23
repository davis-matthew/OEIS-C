/*
 * A175489  a(n) = Sum_{k=1..n} b(k,n), where b(k,n) is the largest positive
 *          integer that, when written in binary, occurs as a substring in
 *          both binary k and binary n.
 * https://oeis.org/A175489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = Σ_{k=1}^n b(k,n), where b(k,n) is the largest positive
 *            integer whose binary representation occurs as a substring in
 *            both binary k and binary n
 * Range:     u8 n <= 40, u16 n <= 1759, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t idx_n = n;

    /* bit length of n */
    idx_t tmp = idx_n;
    int len_n = 0;
    while (tmp) {
        ++len_n;
        tmp >>= 1;
    }

    /* bits array for n (MSB first) */
    uint8_t bits_n[64];
    for (int i = 0; i < len_n; ++i)
        bits_n[i] = (uint8_t)((idx_n >> (len_n - 1 - i)) & 1ULL);

    /* presence array of size n+1 (0 unused) */
    size_t arr_size = (size_t)idx_n + 1;
    uint8_t *present = (uint8_t *)calloc(arr_size, 1);
    if (!present)
        return (seq_t)0;  /* allocation failure */

    /* mark all substrings of n */
    for (int start = 0; start < len_n; ++start) {
        uint64_t val = 0;
        for (int l = 1; l <= len_n - start; ++l) {
            val = (val << 1) | (uint64_t)bits_n[start + l - 1];
            if (val != 0 && (size_t)val < arr_size)
                present[(size_t)val] = 1;
        }
    }

    seq_t sum = (seq_t)0;

    /* iterate over all k ≤ n */
    for (idx_t k = 1; k <= idx_n; ++k) {
        /* bit length of k */
        idx_t tmp_k = k;
        int len_k = 0;
        while (tmp_k) {
            ++len_k;
            tmp_k >>= 1;
        }
        uint8_t bits_k[64];
        for (int i = 0; i < len_k; ++i)
            bits_k[i] = (uint8_t)((k >> (len_k - 1 - i)) & 1ULL);

        uint64_t best = 0;
        for (int start = 0; start < len_k; ++start) {
            uint64_t val = 0;
            for (int l = 1; l <= len_k - start; ++l) {
                val = (val << 1) | (uint64_t)bits_k[start + l - 1];
                if (val != 0 && val > best &&
                    (size_t)val < arr_size && present[(size_t)val])
                    best = val;
            }
        }
        sum += (seq_t)best;
    }

    free(present);
    return sum;
}
