/*
 * A051791  Numbers that are unchanged when turned upside down, when written
 *          in a font in which 7 looks like upside-down 2.
 * https://oeis.org/A051791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   map_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051791: Numbers unchanged when turned upside down
   (7 looks like upside‑down 2). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/map_digit.h"

/* 7^e   (e >= 0) - computed in uint64_t */
static uint64_t pow7(int e)
{
    uint64_t r = 1;
    for (int i = 0; i < e; ++i) r *= 7ULL;
    return r;
}

/* number of strobogrammatic numbers of length L  (L >= 1) */
static uint64_t count_len(idx_t L)
{
    if (L == 1) return 3ULL;
    if ((L & 1ULL) == 0ULL) {                    /* even length */
        return 6ULL * pow7((int)(L / 2 - 1));
    }
    else {                                       /* odd length, L >= 3 */
        return 6ULL * pow7((int)((L - 3) / 2)) * 3ULL;
    }
}

seq_t a(idx_t n)
{
    /* Determine length L of the n-th number */
    idx_t L = 1;
    uint64_t cum = 3ULL;                 /* count_len(1) */
    while (cum < n) {
        ++L;
        cum += count_len(L);
    }
    uint64_t prev_cum = cum - count_len(L);
    uint64_t k = n - prev_cum - 1;       /* 0‑based index within length L */

    /* Radices for positions: first pair (6 choices), other pairs (7), middle (3) */
    const int first_digits[6] = {1, 2, 6, 7, 8, 9};
    const int other_digits[7] = {0, 1, 2, 6, 7, 8, 9};
    const int mid_digits[3]   = {0, 1, 8};

    bool odd = (L & 1);
    int half = L / 2;
    /* suffix[i] = product of radices for positions > i */
    uint64_t suffix[16];
    uint64_t rad[16];
    suffix[half] = odd ? 3ULL : 1ULL;
    for (int i = 0; i < half; ++i)
        rad[i] = (i == 0) ? 6ULL : 7ULL;
    for (int i = half - 1; i >= 0; --i)
        suffix[i] = rad[i] * suffix[i + 1];

    int left[16], right[16];
    uint64_t rem = k;
    for (int i = 0; i < half; ++i) {
        uint64_t step = suffix[i + 1];
        uint64_t idx_choice = rem / step;
        rem = rem % step;
        int d = (i == 0) ? first_digits[idx_choice] : other_digits[idx_choice];
        left[i] = d;
        right[i] = map_digit(d);
    }

    int digits[16];
    for (int i = 0; i < half; ++i) {
        digits[i] = left[i];
        digits[L - 1 - i] = right[i];
    }
    if (odd)
        digits[half] = mid_digits[rem];

    /* Convert the digit array into the numeric value.  Cast through
       unsigned long long so that intermediate products are safe. */
    unsigned long long val = 0ULL;
    for (idx_t i = 0; i < (idx_t)L; ++i)
        val = val * 10ULL + (unsigned long long)digits[i];

    return (seq_t)val;
}
