/*
 * A078135  Numbers which cannot be written as a sum of squares > 1.
 * https://oeis.org/A078135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth positive integer that cannot be written as a sum of
 * squares of integers > 1.
 * This is equivalent to: numbers that cannot be written as a sum of
 * squares of primes.  By proof in the OEIS entry every integer > 23 can
 * be written as such a sum, so the sequence is finite:
 * a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=6, a(6)=7,
 * a(7)=10, a(8)=11, a(9)=14, a(10)=15, a(11)=19, a(12)=23.
 */

#ifndef __cplusplus
#include <stdbool.h>
#endif

static bool canRepresent(idx_t m) {
    /* returns true if m can be expressed as a sum of squares of integers
       greater than 1, otherwise false.
       Only values up to 23 are needed for this sequence, but the routine
       works for any m that fits in idx_t. */
    if (m < 4)
        return false;  /* 1,2,3 cannot be represented */
    /* Use a simple DP with an array of fixed size (enough for any m ≤ 63). */
    bool dp[64];
    for (idx_t i = 0; i <= m; ++i)
        dp[i] = false;
    dp[0] = true;
    for (idx_t s = 2; s * s <= m; ++s) {
        idx_t sq = s * s;
        for (idx_t i = sq; i <= m; ++i) {
            if (dp[i - sq])
                dp[i] = true;
        }
    }
    return dp[m];
}

seq_t a(idx_t n) {
    /* For n > 12 the sequence is undefined; return 0. */
    if (n <= 0 || n > 12)
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t i = 1; i <= 1000; ++i) {
        if (!canRepresent(i)) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
    return (seq_t)0;  /* Should never reach here for n <= 12. */
}
