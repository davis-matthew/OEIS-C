/*
 * A105036  a(n) = 26*a(n-2) - a(n-4) + 12, with a(0) = 0, a(1) = 4, a(2) = 8,
 *          a(3) = 116.
 * https://oeis.org/A105036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-2) - a(n-4) + 12 (n > 3); a(0) = 0, a(1) = 4, a(2)
 *            = 8, a(3) = 116
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#   include <type_traits>
    template <typename T>
    inline seq_t mul26(T x) {
        if constexpr (std::is_same_v<T, uint64_t> || std::is_same_v<T, unsigned long long>) {
            return (seq_t)((unsigned __int128)x * 26);
        } else {
            return x * 26;
        }
    }
#else
    static inline seq_t mul26(seq_t x) {
        return (seq_t)((unsigned __int128)x * 26);
    }
#endif

seq_t a(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t computed = 0;
    static size_t cap = 0;

    /* Ensure base terms are ready */
    if (n < 4) {
        static const seq_t init[4] = {0, 4, 8, 116};
        return init[n];
    }

    /* Allocate or grow the table when needed */
    if (dp == NULL) {
        cap = 16;
        dp = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!dp) return 0;                /* out‑of‑memory: return 0 as fallback */
        dp[0] = 0; dp[1] = 4; dp[2] = 8; dp[3] = 116;
        computed = 4;
    }
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(dp, sizeof(seq_t) * newcap);
        if (!tmp) return 0;                /* out‑of‑memory: return 0 as fallback */
        dp = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val = mul26(dp[i - 2]) - dp[i - 4] + (seq_t)12;
        dp[i] = val;
    }
    computed = n + 1;
    return dp[n];
}
