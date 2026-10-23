/*
 * A164496  Number of binary strings of length n with no substrings equal to
 *          0010 0110 or 1001.
 * https://oeis.org/A164496
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that contain none of
 *            * the forbidden substrings 0010, 0110, or 1001. The sequence
 *            starts at * n = 4. This implementation uses a finite‑state
 *            automaton with 15 * states representing all suffixes of length ≤
 *            3 that have not yet * produced a forbidden patt...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* State information for lengths 0..3 and corresponding bit patterns. */
static const int state_len[15] = {
    0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3
};

static const int state_bits[15] = {
    0, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7
};

/* Indexed by [len][bits] to produce the state ID (0..14).  Unused
   entries are -1. */
static const int state_id[4][8] = {
    { 0, -1, -1, -1, -1, -1, -1, -1 }, /* len = 0, bits only 0 */
    { 1,  2, -1, -1, -1, -1, -1, -1 }, /* len = 1, bits 0..1   */
    { 3,  4,  5,  6, -1, -1, -1, -1 }, /* len = 2, bits 0..3   */
    { 7,  8,  9, 10, 11, 12, 13, 14 }   /* len = 3, bits 0..7   */
};

#if defined(__cplusplus)

/* C++ implementation using std::vector for easier growth. */
static std::vector<seq_t> dp_vec;   /* flattened 1‑D array of size (max_n+1)*15 */
static idx_t max_n = 0;

seq_t a(idx_t n)
{
    const size_t STATES = 15;

    /* Ensure storage is large enough. */
    if (dp_vec.size() < (size_t)(n + 1) * STATES) {
        dp_vec.resize((size_t)(n + 1) * STATES, 0);
        if (max_n == 0) dp_vec[0] = 1;          /* dp[0][0] = 1 */
    }

    /* Compute new rows if necessary. */
    for (idx_t i = max_n; i < n; ++i) {
        size_t row = (size_t)i * STATES;
        for (int idx = 0; idx < (int)STATES; ++idx) {
            seq_t val = dp_vec[row + idx];
            if (!val) continue;

            int l = state_len[idx];
            int bits = state_bits[idx];

            for (int b = 0; b <= 1; ++b) {
                int candidate = (bits << 1) | b; /* 4‑bit value when l == 3 */
                if (l == 3 && (candidate == 2 || candidate == 6 || candidate == 9))
                    continue;                       /* forbidden */

                int newlen = (l == 3) ? 3 : l + 1;
                int newbits = (l == 3) ? (candidate & 7)
                                       : ((bits << 1) | b) & ((1 << newlen) - 1);
                int newidx = state_id[newlen][newbits];
                dp_vec[(size_t)(i + 1) * STATES + newidx] += val;
            }
        }
    }
    max_n = n;

    /* Sum over all end states. */
    seq_t sum = 0;
    size_t end_row = (size_t)n * STATES;
    for (int idx = 0; idx < (int)STATES; ++idx)
        sum += dp_vec[end_row + idx];
    return sum;
}

#else

/* C implementation using malloc/realloc. */
static seq_t (*dp)[15] = NULL;   /* dp[i][s] */
static idx_t max_n = 0;

static void zero_new_rows(idx_t start, idx_t end)
{
    size_t rows = end - start;
    if (rows == 0) return;
    seq_t *ptr = &dp[start][0];
    memset(ptr, 0, sizeof(seq_t) * 15 * rows);
}

seq_t a(idx_t n)
{
    const size_t STATES = 15;

    /* Allocate or expand storage. */
    if (!dp) {
        dp = (seq_t (*)[15])malloc(sizeof(seq_t) * STATES * (n + 1));
        if (!dp) abort();
        memset(dp, 0, sizeof(seq_t) * STATES * (n + 1));
        dp[0][0] = 1;          /* initial state */
        max_n = 0;
    } else if (n > max_n) {
        seq_t (*new_dp)[15] = (seq_t (*)[15])
            realloc(dp, sizeof(seq_t) * STATES * (n + 1));
        if (!new_dp) abort();
        dp = new_dp;
        zero_new_rows(max_n + 1, n + 1);
    }

    /* Compute new rows if needed. */
    for (idx_t i = max_n; i < n; ++i) {
        for (int idx = 0; idx < (int)STATES; ++idx) {
            seq_t val = dp[i][idx];
            if (!val) continue;

            int l = state_len[idx];
            int bits = state_bits[idx];

            for (int b = 0; b <= 1; ++b) {
                int candidate = (bits << 1) | b;
                if (l == 3 && (candidate == 2 || candidate == 6 || candidate == 9))
                    continue;                       /* forbidden */

                int newlen = (l == 3) ? 3 : l + 1;
                int newbits = (l == 3) ? (candidate & 7)
                                       : ((bits << 1) | b) & ((1 << newlen) - 1);
                int newidx = state_id[newlen][newbits];
                dp[i + 1][newidx] += val;
            }
        }
    }
    max_n = n;

    /* Sum over all end states. */
    seq_t sum = 0;
    for (int idx = 0; idx < (int)STATES; ++idx)
        sum += dp[n][idx];
    return sum;
}

#endif
