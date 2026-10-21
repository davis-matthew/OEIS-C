/*
 * A207954  Palindromes with consecutive digits that differ exactly by 1.
 * https://oeis.org/A207954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth palindrome whose adjacent digits differ by
 *            exactly 1. Only palindromes of odd length can satisfy the
 *            adjacency requirement
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 235, u64 n <= 6236,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#ifndef MAX_M
#define MAX_M 12   /* sufficient for n up to at least 10000 (length 21) */
#endif

static bool initialized = false;
static idx_t f[MAX_M+1][10];       /* f[k][d] = number of walks of length k starting at digit d */
static idx_t count_len[MAX_M+1];   /* number of palindromes of length 2*m+1 (m = step) */
static idx_t cum[MAX_M+1];         /* cumulative counts up to step m (inclusive) */

static void init_tables(void)
{
    /* f[0][d] = 1 for all d: a sequence of length 1 */
    for (int d = 0; d <= 9; ++d)
        f[0][d] = 1;

    count_len[0] = 0;
    for (int d = 1; d <= 9; ++d)
        count_len[0] += f[0][d];
    cum[0] = count_len[0];

    for (int step = 1; step <= MAX_M; ++step) {
        for (int d = 0; d <= 9; ++d) {
            idx_t val = 0;
            if (d > 0)   val += f[step-1][d-1];
            if (d < 9)   val += f[step-1][d+1];
            f[step][d] = val;
        }
        count_len[step] = 0;
        for (int d = 1; d <= 9; ++d)
            count_len[step] += f[step][d];
        cum[step] = cum[step-1] + count_len[step];
    }
    initialized = true;
}

/* Computes the nth term of OEIS A207954. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_tables();

    /* First nine terms are the single–digit numbers 1..9. */
    if (n <= 9)
        return (seq_t)n;

    /* Find the odd‑length group containing the nth term. */
    int m = 0;                       /* m such that length = 2*m+1 */
    while (m <= MAX_M && cum[m] < n)
        ++m;

    idx_t k = n - cum[m-1];          /* 1‑based index within the group of length 2*m+1 */

    /* Build the left half + middle (m+1 digits). */
    int digits[MAX_M+1];
    int rem = m;                    /* number of steps remaining after choosing current digit */

    /* First digit (most significant) cannot be zero. */
    for (int d = 1; d <= 9; ++d) {
        idx_t cnt = f[rem][d];
        if (k > cnt) {
            k -= cnt;
        } else {
            digits[0] = d;
            rem--;                 /* consume the step for choosing this digit */
            break;
        }
    }

    /* Subsequent digits, always staying within 0..9 and differing by 1. */
    for (int pos = 1; pos <= m; ++pos) {
        int prev = digits[pos-1];
        /* Consider the possible neighbors in ascending order. */
        if (prev > 0) {
            int cand = prev - 1;
            idx_t cnt = f[rem][cand];
            if (k <= cnt) {
                digits[pos] = cand;
                rem--;
                continue;
            } else
                k -= cnt;
        }
        if (prev < 9) {
            int cand = prev + 1;
            idx_t cnt = f[rem][cand];
            if (k <= cnt) {
                digits[pos] = cand;
                rem--;
                continue;
            } else
                k -= cnt;
        }
    }

    /* Construct the palindrome from the digits. */
    seq_t value = 0;
    for (int i = 0; i <= m; ++i)
        value = value * 10 + (seq_t)digits[i];
    for (int i = m - 1; i >= 0; --i)
        value = value * 10 + (seq_t)digits[i];

    return value;
}
