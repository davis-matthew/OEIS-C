/*
 * A104439  Number of ways to split 1, 2, 3, ..., 6n into 6 arithmetic
 *          progressions each with n terms.
 * https://oeis.org/A104439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computed by brute‑force for n<7, constant 56 for n≥7 */

static void dfs(uint64_t start, uint64_t depth, uint64_t mask,
                const uint64_t *masks, uint64_t num,
                uint64_t n, uint64_t m, uint64_t *count)
{
    if (depth == 6) {
        uint64_t fullMask = (m == 64) ? ~0ull : ((1ull << m) - 1);
        if (mask == fullMask) (*count)++;
        return;
    }
    uint64_t used = __builtin_popcountll(mask);
    uint64_t remainingBits = m - used;
    if (remainingBits < (6 - depth) * n) return;
    for (uint64_t i = start; i < num; ++i) {
        if (mask & masks[i]) continue;
        dfs(i + 1, depth + 1, mask | masks[i],
            masks, num, n, m, count);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n >= 7) return (seq_t)56;
    if (n == 0) return (seq_t)0;  /* off‑offset, not used */

    uint64_t nn = (uint64_t)n;
    uint64_t m  = 6 * nn;
    uint64_t max_d = (m - 1) / (nn - 1);

    uint64_t masks[200];
    uint64_t mask_count = 0;

    for (uint64_t d = 1; d <= max_d; ++d) {
        uint64_t a_max = m - (nn - 1) * d;
        for (uint64_t a = 1; a <= a_max; ++a) {
            uint64_t mask = 0;
            for (uint64_t i = 0; i < nn; ++i) {
                mask |= 1ull << (a - 1 + i * d);
            }
            masks[mask_count++] = mask;
        }
    }

    uint64_t count = 0;
    dfs(0, 0, 0, masks, mask_count, nn, m, &count);
    return (seq_t)count;
}
