/*
 * A082935  Smallest palindrome beginning with n and a digit sum of n at some
 *          stage.
 * https://oeis.org/A082935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   computes the smallest palindrome beginning with n whose repeated
 *            digit–sum process eventually reaches n
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 175,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../utils/digit_sum.h"

/* helper: does repeated digit sum of s reach target? */
static bool reaches_n(seq_t s, seq_t target)
{
    seq_t cur = s;
    for (int iter = 0; iter < 100; ++iter) {
        if (cur == target) return true;
        if (cur < 10) return false;
        cur = digit_sum(cur);
    }
    return false;
}

seq_t a(idx_t n)
{
    /* convert n to decimal string */
    char nstr[25];
    snprintf(nstr, sizeof nstr, "%llu", (unsigned long long)n);
    int t = (int)strlen(nstr);
    int ndigits[25];
    for (int i = 0; i < t; ++i) ndigits[i] = nstr[i] - '0';

    seq_t target = (seq_t)n;

    /* search by palindrome length */
    int L = t;
    const int MAX_SEARCH_LEN = 2000;   /* generous upper bound */
    for (; L < MAX_SEARCH_LEN; ++L) {
        int left_len = (L + 1) / 2;
        /* left side digits, -1 means undefined */
        int left[256];
        for (int i = 0; i < left_len; ++i) left[i] = -1;

        /* apply constraints from n */
        bool conflict = false;
        for (int i = 0; i < t; ++i) {
            int d = ndigits[i];
            int pos = (i < left_len) ? i : (L - 1 - i);
            if (left[pos] != -1 && left[pos] != d) { conflict = true; break; }
            left[pos] = d;
        }
        if (conflict) continue;

        /* sum of digits of the palindrome given current fixed digits */
        seq_t sum_prefix = 0;
        for (int pos = 0; pos < left_len; ++pos) {
            if (left[pos] == -1) continue;
            int d = left[pos];
            sum_prefix += (seq_t)d;
            int mirror = L - 1 - pos;
            if (mirror != pos) sum_prefix += (seq_t)d; /* mirrored digit */
        }

        /* list of free positions and their weights */
        int free_pos[256];
        int free_wts[256];
        int free_cnt = 0;
        for (int pos = 0; pos < left_len; ++pos) {
            if (left[pos] != -1) continue;
            int weight = (pos == left_len - 1 && (L & 1)) ? 1 : 2;
            free_pos[free_cnt] = pos;
            free_wts[free_cnt] = weight;
            ++free_cnt;
        }

        /* maximum contribution from free digits */
        seq_t max_contrib = 0;
        for (int i = 0; i < free_cnt; ++i) {
            max_contrib += (seq_t)free_wts[i] * 9;
        }

        /* iterate possible first digit sum (sum of digits of palindrome) */
        for (seq_t sum_needed = sum_prefix;
             sum_needed <= sum_prefix + max_contrib;
             ++sum_needed) {

            if (!reaches_n(sum_needed, target)) continue;

            /* residual to distribute among free digits */
            seq_t residual = sum_needed - sum_prefix;
            if (residual > max_contrib) continue;  /* safety */

            /* suffix maximum array for greedy feasibility test */
            seq_t suffix_max[257];
            suffix_max[free_cnt] = 0;
            for (int i = free_cnt - 1; i >= 0; --i)
                suffix_max[i] = suffix_max[i + 1] + (seq_t)free_wts[i] * 9;

            /* greedy assign minimal digits */
            seq_t rem = residual;
            bool bad = false;
            for (int i = 0; i < free_cnt; ++i) {
                int weight = free_wts[i];
                int pos = free_pos[i];
                int chosen = -1;
                for (int d = 0; d <= 9; ++d) {
                    seq_t rem_after = rem - (seq_t)weight * d;
                    if (rem_after < 0) break;
                    if (rem_after <= suffix_max[i + 1]) {
                        chosen = d;
                        rem = rem_after;
                        break;
                    }
                }
                if (chosen == -1) { bad = true; break; }
                left[pos] = chosen;
            }
            if (bad || rem != 0) continue;  /* should not happen */

            /* build full palindrome digits */
            int digits[256];
            for (int i = 0; i < left_len; ++i) digits[i] = left[i];
            for (int i = left_len; i < L; ++i)
                digits[i] = digits[L - 1 - i];

            /* convert to seq_t */
            seq_t result = 0;
            for (int i = 0; i < L; ++i) {
                result = result * 10 + (seq_t)digits[i];
            }
            return result;
        }
    }
    /* should never reach here for n in the valid range */
    return 0;
}
