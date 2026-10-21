/*
 * A226970  Fixed points for the operation of repeatedly replacing a number
 *          with the sum of the ninth powers of its digits.
 * https://oeis.org/A226970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the nth integer that equals the sum of the ninth powers
 *            of its digits
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   freq_matches
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/freq_matches.h"

static const seq_t pow9[10] = {
    (seq_t)0,              // 0^9
    (seq_t)1,              // 1^9
    (seq_t)512,            // 2^9
    (seq_t)19683,          // 3^9
    (seq_t)262144,         // 4^9
    (seq_t)1953125,        // 5^9
    (seq_t)10077696,       // 6^9
    (seq_t)40353607,       // 7^9
    (seq_t)134217728,      // 8^9
    (seq_t)387420489       // 9^9
};

/* Generate all fixed points of digit^9 sum and store them in a sorted array. */
static seq_t fixed_points[6];
static size_t fp_count = 0;

/* Recursively enumerate digit counts for a given number of digits. */
static void enumerate_counts(int digit, int remaining, unsigned char *cnt, int d)
{
    if (digit == 9) {
        cnt[9] = remaining;
        /* Compute sum of ninth powers. */
        seq_t sum = 0;
        for (int i = 0; i < 10; ++i) sum += (seq_t)cnt[i] * pow9[i];

        /* Determine length and digit frequencies of sum. */
        unsigned char freq[10] = {0};
        int len = 0;
        seq_t tmp = sum;
        if (tmp == 0) {
            len = 1;
            freq[0] = 1;
        } else {
            while (tmp > 0) {
                int dgt = (int)(tmp % 10);
                freq[dgt]++;
                tmp /= 10;
                len++;
            }
        }

        if (len != d) return; /* Number of digits must match. */
        if (!freq_matches(cnt, freq, d)) return;

        /* Store the fixed point. */
        if (fp_count < 6) {
            fixed_points[fp_count++] = sum;
        }
        return;
    }

    for (int c = 0; c <= remaining; ++c) {
        cnt[digit] = (unsigned char)c;
        enumerate_counts(digit + 1, remaining - c, cnt, d);
    }
}

/* Sort the fixed point array (simple insertion sort; size <= 6). */
static void sort_fixed_points(void)
{
    for (size_t i = 1; i < fp_count; ++i) {
        seq_t key = fixed_points[i];
        size_t j = i;
        while (j > 0 && fixed_points[j - 1] > key) {
            fixed_points[j] = fixed_points[j - 1];
            --j;
        }
        fixed_points[j] = key;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* Compute all fixed points (max up to 10 digits). */
        unsigned char cnt[10] = {0};
        for (int d = 1; d <= 10; ++d) {
            enumerate_counts(0, d, cnt, d);
        }
        sort_fixed_points();
        initialized = true;
    }

    if (n == 0) {
        /* Offset starts at 1; undefined for n=0. Return 0 for safety. */
        return (seq_t)0;
    }

    idx_t idx = n - 1; /* convert to 0-based index */
    if (idx < (idx_t)fp_count) {
        return fixed_points[idx];
    }
    return (seq_t)0; /* no further terms defined */
}
