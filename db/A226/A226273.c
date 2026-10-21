/*
 * A226273  Number of distinct values u^v, where u and v are digits occurring
 *          in decimal representation of n.
 * https://oeis.org/A226273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct values u^v, where u and v are digits
 *            occurring in the decimal representation of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper: compute u^v as uint64_t (v >= 0) */
static uint64_t power_u64(unsigned int u, unsigned int v)
{
    uint64_t result = 1;
    for (unsigned int i = 0; i < v; ++i)
        result *= (uint64_t)u;
    return result;
}

seq_t a(idx_t n)
{
    /* 1. Determine which decimal digits appear in n */
    bool present[10] = { false };
    if (n == 0) {
        present[0] = true;
    } else {
        idx_t tmp = n;
        while (tmp != 0) {
            present[(unsigned)(tmp % 10)] = true;
            tmp /= 10;
        }
    }

    /* 2. Enumerate all u^v for present digits and count distinct values */
    uint64_t seen[100];
    size_t seen_cnt = 0;

    for (int u = 0; u < 10; ++u) {
        if (!present[u]) continue;
        for (int v = 0; v < 10; ++v) {
            if (!present[v]) continue;
            uint64_t val = power_u64(u, v);
            bool duplicate = false;
            for (size_t i = 0; i < seen_cnt; ++i) {
                if (seen[i] == val) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                seen[seen_cnt++] = val;
            }
        }
    }

    return (seq_t)seen_cnt;
}
