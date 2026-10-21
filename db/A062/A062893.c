/*
 * A062893  Number of squares that can be obtained by using some or all of the
 *          digits of n.
 * https://oeis.org/A062893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = number of distinct perfect squares that can be formed
 *            using some or all of the digits of n (in any order)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* integer square root (floor) for 64‑bit unsigned integers */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/* next lexicographic permutation of an array of integers.
   Returns false if the array is in the last (highest) permutation. */
static bool next_perm(int *arr, size_t len)
{
    if (len < 2) return false;
    int i = (int)len - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)len - 1;
    while (arr[j] <= arr[i]) j--;
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
    int l = i + 1, r = (int)len - 1;
    while (l < r) {
        tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
        l++; r--;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Extract digits of n into array digits[0..d-1] (least significant first) */
    uint8_t digits[20];          /* enough for any n up to 10⁵ */
    size_t d = 0;
    if (n == 0) {
        digits[0] = 0;
        d = 1;
    } else {
        idx_t tmp = n;
        while (tmp) {
            digits[d++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Store distinct squares found; maximum possible is small */
    uint64_t sq[4000];
    size_t sq_cnt = 0;

    /* Iterate over all non‑empty subsets of the d digits */
    for (size_t mask = 1; mask < (1ULL << d); mask++) {
        /* Collect digits of this subset */
        int subset[20];
        size_t k = 0;
        for (size_t i = 0; i < d; i++)
            if (mask & (1ULL << i))
                subset[k++] = digits[i];
        if (k == 0) continue;   /* shouldn't happen */

        /* Sort to prepare for permutation generation */
        for (size_t i = 0; i < k; i++)
            for (size_t j = i + 1; j < k; j++)
                if (subset[i] > subset[j]) {
                    int tmp = subset[i];
                    subset[i] = subset[j];
                    subset[j] = tmp;
                }

        /* Enumerate all permutations of subset */
        do {
            /* Build number from digits (most significant first) */
            uint64_t num = 0;
            for (size_t i = 0; i < k; i++)
                num = num * 10 + (uint64_t)subset[i];

            /* Check if num is a perfect square */
            uint64_t r = isqrt_u64(num);
            if (r * r == num) {
                /* Add to list if not already present */
                bool present = false;
                for (size_t i = 0; i < sq_cnt; i++)
                    if (sq[i] == num) { present = true; break; }
                if (!present) sq[sq_cnt++] = num;
            }
        } while (next_perm(subset, k));
    }

    return (seq_t)sq_cnt;
}
