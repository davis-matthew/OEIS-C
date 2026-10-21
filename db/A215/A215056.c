/*
 * A215056  a(n) = smallest k such that k is both a sum of n nonzero squares
 *          and 50-n nonzero squares.
 * https://oeis.org/A215056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the smallest k that can be expressed as a sum of
   n non‑zero squares and also as a sum of 50‑n non‑zero squares.
   The function precomputes reachability of sums using a dynamic
   programming table for all counts 0…50 up to a fixed maximum sum
   (MAX_K).  Then for each requested n it returns the first sum that
   appears in both counts.  The algorithm is O(MAX_K·sqrt(MAX_K)) and
   is fast for all n ≤ 49. */

#include <stdint.h>
#include <stddef.h>

/* Maximum sum that will be examined.  The sequence values are all
   below 200, so 2000 is more than safe for all n. */
#define MAX_K 2000
#define WORDS ((MAX_K + 63) / 64)

/* Shift the bitset 'src' by 'shift' bits to the left and OR the result
   into the bitset 'dest'.  Both bitsets have WORDS elements. */
static void shift_or(uint64_t *dest, const uint64_t *src, unsigned shift)
{
    unsigned word_off = shift >> 6;          /* shift / 64 */
    unsigned bit_off  = shift & 63;          /* shift % 64 */
    if (word_off >= WORDS) return;           /* shift beyond range */

    if (bit_off == 0) {
        for (size_t i = WORDS; i-- > word_off; ) {
            dest[i] |= src[i - word_off];
        }
    } else {
        for (size_t i = WORDS; i-- > word_off; ) {
            uint64_t high = src[i - word_off] << bit_off;
            uint64_t low  = 0;
            if (i - word_off > 0)
                low = src[i - word_off - 1] >> (64 - bit_off);
            dest[i] |= high | low;
        }
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* The sequence is only defined for 1 ≤ n ≤ 49.  For any other
       value we return 0. */
    if (n < 1 || n > 49) return 0;

    /* Static data that is computed only once. */
    static int initialized = 0;
    static seq_t answers[50] = {0};                     /* answers[n-1] */
    static uint64_t dp[51][WORDS] = {0};

    if (!initialized) {
        /* Base: zero squares sum to 0 */
        dp[0][0] = 1ULL;

        /* Unlimited use of each square value up to MAX_K. */
        for (int v = 1; ; ++v) {
            int sq = v * v;
            if (sq > MAX_K) break;
            for (int cnt = 1; cnt <= 50; ++cnt) {
                shift_or(dp[cnt], dp[cnt-1], sq);
            }
        }

        /* Find minimal k that lies in both dp[n] and dp[50-n]. */
        int remaining = 49;
        for (int k = 1; k <= MAX_K && remaining > 0; ++k) {
            size_t w = k >> 6;
            unsigned b = k & 63;
            for (int i = 1; i <= 49; ++i) {
                if (!answers[i-1]) {
                    if ( ((dp[i][w] >> b) & 1ULL) &&
                         ((dp[50-i][w] >> b) & 1ULL) )
                    {
                        answers[i-1] = (seq_t)k;
                        --remaining;
                    }
                }
            }
        }
        initialized = 1;
    }

    return answers[n-1];
}
