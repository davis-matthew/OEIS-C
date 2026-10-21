/*
 * A227238  Numbers whose base-9 sum of digits is 9.
 * https://oeis.org/A227238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 875, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227238 – Numbers whose base‑9 digit sum equals 9.
   The nth term is found by enumerating the digit strings of increasing
   length.  For a given length L the number of admissible strings is
   C(L+7, L-1) – 1.  The strings are ordered lexicographically, which
   coincides with increasing decimal value.  The function un-ranks the
   k‑th string of a fixed length using standard combinatorial formulas. */

#include <stdint.h>

/* binomial coefficient n choose k, computed with 128‑bit arithmetic. */
static inline unsigned __int128 binom128(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return res;
}

/* Count of digit strings of length L (≥2) whose digits sum to 9.
   The formula: C(L+7, L-1) – 1.  The result is capped at UINT64_MAX.
 */
static inline uint64_t count_len(uint64_t L) {
    unsigned __int128 c = binom128(L + 7, L - 1);
    if (c == 0) return 0;
    c -= 1;
    if (c > (unsigned __int128)UINT64_MAX) return UINT64_MAX;
    return (uint64_t)c;
}

/* Count of weak compositions of sum S into R parts (each part 0..8, but
   S ≤ 8 so the upper bound is automatic).  Formula: C(S+R-1, R-1). */
static inline uint64_t count_compositions(uint64_t S, uint64_t R) {
    unsigned __int128 c = binom128(S + R - 1, R - 1);
    if (c > (unsigned __int128)UINT64_MAX) return UINT64_MAX;
    return (uint64_t)c;
}

/* Un‑rank the k‑th lexicographic digit string of length L that
   sums to 9.  The result is returned in decimal (base 10) as a seq_t. */
static seq_t unrank_len(uint64_t L, uint64_t k) {
    uint64_t remSum = 9;
    seq_t value = 0;

    for (uint64_t pos = 0; pos < L; ++pos) {
        uint64_t minDigit = (pos == 0) ? 1 : 0;
        uint64_t maxDigit = 8;           /* all digits ≤ 8 */

        for (uint64_t d = minDigit; d <= maxDigit; ++d) {
            if (remSum < d) break;
            uint64_t remAfter = remSum - d;
            uint64_t remainPos = L - pos - 1;
            uint64_t ways;

            if (remainPos == 0) {                     /* last position */
                ways = (remAfter == 0) ? 1 : 0;
            } else {
                ways = count_compositions(remAfter, remainPos);
            }

            if (k > ways) {
                k -= ways;
            } else {
                value = value * 9 + (seq_t)d;
                remSum = remAfter;
                break;
            }
        }
    }
    return value;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n = 0 */

    uint64_t remaining = (uint64_t)n;

    for (uint64_t L = 2; ; ++L) {
        uint64_t cnt = count_len(L);
        if (remaining <= cnt) {
            return unrank_len(L, remaining);
        }
        remaining -= cnt;
    }
}
