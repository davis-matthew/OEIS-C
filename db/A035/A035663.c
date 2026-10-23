/*
 * A035663  Number of partitions of n into parts 7k+2 and 7k+4 with at least
 *          one part of each type.
 * https://oeis.org/A035663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts 7k+2 and 7k+4 * with at
 *            least one part of each type
 * Range:     u8 n <= 55, u16 n <= 143, u32 n <= 423, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple DP that counts unrestricted
 * partitions for each type separately and then subtracts the
 * partitions that use only one type.  All values are stored in
 * static tables that are recomputed whenever a larger n is
 * requested.  The tables are resized and re‑initialized
 * (including construction of any non‑POD element type) when
 * compiled as C++17. */

/* Standard headers */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Global tables and current maximum n */
#if defined(__cplusplus)
static std::vector<seq_t> dp_f, dp_f2, dp_f4;
#else
static seq_t *dp_f  = NULL;
static seq_t *dp_f2 = NULL;
static seq_t *dp_f4 = NULL;
#endif
static idx_t max_n = 0;

/* Resize tables to handle all n ≤ new_n and recompute DP tables. */
static void expand(idx_t new_n)
{
    if (new_n <= max_n) return;          /* No update required */

    /* Allocate and initialise tables */
#if defined(__cplusplus)
    dp_f.assign((size_t)new_n + 1, 0);
    dp_f2.assign((size_t)new_n + 1, 0);
    dp_f4.assign((size_t)new_n + 1, 0);
    dp_f[0]  = 1;
    dp_f2[0] = 1;
    dp_f4[0] = 1;
#else
    size_t cap = (size_t)new_n + 1;
    dp_f  = (seq_t *)realloc(dp_f,  cap * sizeof(seq_t));
    dp_f2 = (seq_t *)realloc(dp_f2, cap * sizeof(seq_t));
    dp_f4 = (seq_t *)realloc(dp_f4, cap * sizeof(seq_t));
    for (size_t i = 0; i < cap; ++i) {
        dp_f[i]  = 0;
        dp_f2[i] = 0;
        dp_f4[i] = 0;
    }
    dp_f[0]  = 1;
    dp_f2[0] = 1;
    dp_f4[0] = 1;
#endif

    /* DP for type 7k+2 only */
    for (idx_t s = 2; s <= new_n; s += 7) {
        for (idx_t i = s; i <= new_n; ++i) {
#if defined(__cplusplus)
            dp_f2[i] += dp_f2[i - s];
#else
            dp_f2[i] += dp_f2[i - s];
#endif
        }
    }

    /* DP for type 7k+4 only */
    for (idx_t s = 4; s <= new_n; s += 7) {
        for (idx_t i = s; i <= new_n; ++i) {
#if defined(__cplusplus)
            dp_f4[i] += dp_f4[i - s];
#else
            dp_f4[i] += dp_f4[i - s];
#endif
        }
    }

    /* DP for all parts (both types) */
    for (idx_t s = 2; s <= new_n; s += 7) {
        for (idx_t i = s; i <= new_n; ++i) {
#if defined(__cplusplus)
            dp_f[i] += dp_f[i - s];
#else
            dp_f[i] += dp_f[i - s];
#endif
        }
    }
    for (idx_t s = 4; s <= new_n; s += 7) {
        for (idx_t i = s; i <= new_n; ++i) {
#if defined(__cplusplus)
            dp_f[i] += dp_f[i - s];
#else
            dp_f[i] += dp_f[i - s];
#endif
        }
    }

    max_n = new_n;
}

/* Public function required by the task */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    expand(n);
#if defined(__cplusplus)
    return dp_f[(size_t)n] - dp_f2[(size_t)n] - dp_f4[(size_t)n];
#else
    return dp_f[n] - dp_f2[n] - dp_f4[n];
#endif
}
