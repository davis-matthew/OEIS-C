/*
 * A067590  Number of partitions of n into odious numbers (A000069).
 * https://oeis.org/A067590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into odious numbers (odious =
 *            integers with an odd number of 1‑bits in binary)
 * Range:     u8 n <= 25, u16 n <= 70, u32 n <= 222, u64 n <= 760,
 *            u128 n <= 2753, big any n
 * Helpers:   popcnt64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the standard coin‑change DP:
      dp[0] = 1
      dp[i] = Σ_{c odious ≤ i} dp[i‑c]
   It works for all n ≥ 0.  The implementation uses a static
   array of length 10001 for typical values (n ≤ 10000).  For
   larger n it falls back to a dynamic array allocated with malloc.
   The code is valid C11 and C++17, so it compiles in either mode. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/popcnt64.h"

/* Static array for n up to 10000.  The size is a compile‑time constant
   so it works in both C and C++. */
static seq_t dp_static[10001];

seq_t a(idx_t n)
{
    /* Use static array for typical cases. */
    if (n <= 10000) {
        size_t len = (size_t)n + 1;
        for (size_t i = 0; i < len; ++i)
            dp_static[i] = (seq_t)0;
        dp_static[0] = (seq_t)1;

        for (idx_t coin = 1; coin <= n; ++coin) {
            if ((popcnt64((uint64_t)coin) & 1) == 0)  /* not odious */
                continue;
            for (size_t i = (size_t)coin; i < len; ++i)
                dp_static[i] += dp_static[i - (size_t)coin];
        }
        return dp_static[(size_t)n];
    }

    /* For n > 10000, allocate a dynamic array.  This path may be
       slower but covers the full requirement. */
    size_t len = (size_t)n + 1;
    seq_t *dp_dyn = (seq_t*)malloc(len * sizeof(seq_t));
    if (!dp_dyn) return (seq_t)0;  /* allocation failed */

    for (size_t i = 0; i < len; ++i)
        dp_dyn[i] = (seq_t)0;
    dp_dyn[0] = (seq_t)1;

    for (idx_t coin = 1; coin <= n; ++coin) {
        if ((popcnt64((uint64_t)coin) & 1) == 0)
            continue;
        for (size_t i = (size_t)coin; i < len; ++i)
            dp_dyn[i] += dp_dyn[i - (size_t)coin];
    }

    seq_t result = dp_dyn[(size_t)n];
    free(dp_dyn);
    return result;
}
