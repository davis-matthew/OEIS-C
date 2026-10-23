/*
 * A032981  Positive numbers with the property that all pairs of consecutive
 *          base-10 digits differ by 0 or 1.
 * https://oeis.org/A032981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) enumerates the n-th positive integer whose consecutive
 *            decimal digits differ by at most 1
 * Range:     u8 n <= 52, u16 n <= 714, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static unsigned long long F[256][10];
static bool initialized = false;

/* Precompute the number of ways to append `steps` digits after a given last
   digit `d`. */
static void init_F(void)
{
    if (initialized) return;

    for (int d = 0; d < 10; ++d) F[0][d] = 1ULL;

    for (int steps = 1; steps < 256; ++steps) {
        for (int d = 0; d < 10; ++d) {
            unsigned long long sum = 0ULL;
            int lo = d - 1;
            int hi = d + 1;
            if (lo < 0) lo = 0;
            if (hi > 9) hi = 9;
            for (int nd = lo; nd <= hi; ++nd) sum += F[steps - 1][nd];
            F[steps][d] = sum;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; behavior below offset is irrelevant. */
    if (n == 0) return (seq_t)0;

    init_F();

    unsigned long long cumulative = 0ULL;
    int len = 1;
    unsigned long long count_len;

    /* Find the length `len` such that the a(n) lies within the numbers of that length. */
    while (true) {
        count_len = 0ULL;
        for (int d = 1; d <= 9; ++d) count_len += F[len - 1][d];
        if (cumulative + count_len >= (unsigned long long)n) break;
        cumulative += count_len;
        ++len;
    }

    unsigned long long offset = (unsigned long long)n - cumulative - 1ULL;  /* zero‑based index within this length */

    seq_t result = 0;
    int prev_digit = -1;
    int remaining = len - 1;   /* digits still to determine after the current one */

    for (int pos = 0; pos < len; ++pos) {
        int start_digit = (pos == 0) ? 1 : 0;   /* leading digit cannot be zero */
        for (int d = start_digit; d <= 9; ++d) {
            if (pos > 0 && (d > prev_digit + 1 || d < prev_digit - 1)) continue;

            unsigned long long cnt = F[remaining][d];
            if (offset < cnt) {
                result = (result * 10ULL) + (seq_t)d;
                prev_digit = d;
                --remaining;
                break;
            } else {
                offset -= cnt;
            }
        }
    }
    return result;
}
