/*
 * A057907  Positive integers that are not the sum of precisely six positive
 *          cubes.
 * https://oeis.org/A057907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 492, u32 n <= 492, u64 n <= 492,
 *            u128 n <= 492, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th positive integer that cannot be expressed as a sum
 *               of exactly six positive cubes.
 *
 * Implementation:  brute‑force DP up to a hard‑coded limit that covers the
 *                   entire known range of the sequence (19202).  This is
 *                   sufficient for all tested values (492 terms).
 *                   The routine is memoised so that repeated calls are
 *                   O(1) after the initial build.
 */
#include <stdbool.h>

#define MAX_SUM 20000   /* Upper bound that contains all known terms (19202) */

static bool initialized = false;
static idx_t missing_count = 0;
static seq_t missing_numbers[MAX_SUM + 1];

/* build_missing_numbers precomputes all integers ≤ MAX_SUM that are not
 * representable as a sum of exactly six positive cubes. */
static void build_missing_numbers(void)
{
    bool dp[7][MAX_SUM + 1] = { false };
    dp[0][0] = true;

    /* iterate over all possible cubes ≤ MAX_SUM */
    for (idx_t c = 1; c * c * c <= MAX_SUM; ++c) {
        idx_t cube = c * c * c;   /* cube fits in idx_t (≤ MAX_SUM) */

        /* update DP for each possible count of cubes used */
        for (idx_t cnt = 1; cnt <= 6; ++cnt) {
            /* sum can start at 'cube' because we need at least one cube of
             * that size */
            for (idx_t sum = cube; sum <= MAX_SUM; ++sum) {
                if (dp[cnt - 1][sum - cube])
                    dp[cnt][sum] = true;
            }
        }
    }

    /* collect numbers that are NOT representable as a sum of 6 cubes */
    missing_count = 0;
    for (idx_t sum = 1; sum <= MAX_SUM; ++sum) {
        if (!dp[6][sum]) {
            if (missing_count <= MAX_SUM) {
                missing_numbers[missing_count] = (seq_t)sum;
            }
            ++missing_count;
        }
    }

    initialized = true;
}

/* a(n) returns the n-th term of the sequence, for n ≥ 1.  Calls with n=0
 * return 0 for safety.  Calls beyond the known number of terms return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* guard against 0‑based misuse */

    if (!initialized)
        build_missing_numbers();

    if (n - 1 < missing_count)
        return missing_numbers[n - 1];

    return 0;   /* undefined for n larger than the precomputed range */
}
