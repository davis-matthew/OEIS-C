/*
 * A000123  Number of binary partitions: number of partitions of 2n into
 *          powers of 2.
 * https://oeis.org/A000123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = a(n-1) + a(floor(n/2)), a(0) = 1
 * Range:     u8 n <= 17, u16 n <= 78, u32 n <= 561, u64 n <= 7862,
 *            u128 n <= 9693, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>

seq_t a(idx_t n)
{
    static std::vector<seq_t> memo;
    if (memo.empty())
        memo.push_back((seq_t)1);
    while ((idx_t)memo.size() <= n) {
        size_t k = memo.size();
        seq_t v = memo[k - 1] + memo[k / 2];
        memo.push_back(v);
    }
    return memo[(size_t)n];
}

#else

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t len = 0, cap = 0;
    if (len == 0) {
        cap = 1024;
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        memo[0] = (seq_t)1;
        len = 1;
    }
    if ((idx_t)len <= n) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t nc = cap;
            while (nc < need)
                nc *= 2;
            memo = (seq_t *)realloc(memo, nc * sizeof(seq_t));
            cap = nc;
        }
        while (len < need) {
            memo[len] = memo[len - 1] + memo[len / 2];
            len++;
        }
    }
    return memo[(size_t)n];
}

#endif
