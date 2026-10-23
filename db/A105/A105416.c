/*
 * A105416  Numbers that are pandigital in Roman numerals using each of the
 *          symbols I, V, X, L, C, D and M exactly once.
 * https://oeis.org/A105416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   Enumerates all permutations of I,V,X,L,C,D,M and returns the
 *            n-th smallest valid Roman numeral value, i.e., numbers whose
 *            Roman representation uses each of the symbols I, V, X, L, C, D,
 *            M exactly once
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   allowed_subtractive, compute_value, value_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/allowed_subtractive.h"
#include "../utils/compute_value.h"
#include "../utils/value_of.h"

static const char symbols[7] = {'I','V','X','L','C','D','M'};
static const int   values_map[7] = {1,5,10,50,100,500,1000};

/* Backtracking to generate all permutations and collect valid values */
static void backtrack(int pos, bool used[7], char perm[7],
                      seq_t *out_vals, size_t *out_cnt) {
    if (pos == 7) {
        seq_t val = compute_value(perm);
        if (val != 0) {
            out_vals[*out_cnt] = val;
            (*out_cnt)++;
        }
        return;
    }
    for (int i = 0; i < 7; ++i) {
        if (!used[i]) {
            used[i] = true;
            perm[pos] = symbols[i];
            backtrack(pos + 1, used, perm, out_vals, out_cnt);
            used[i] = false;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* static storage for all terms */
    static seq_t all_vals[5040];
    static idx_t all_len = 0;

    if (all_len == 0) {
        bool used[7] = {0};
        char perm[7];
        size_t cnt = 0;
        backtrack(0, used, perm, all_vals, &cnt);

        /* Simple O(n^2) sort (n <= 5040) */
        for (size_t i = 0; i + 1 < cnt; ++i)
            for (size_t j = cnt - 1; j > i; --j)
                if (all_vals[j] < all_vals[j-1]) {
                    seq_t tmp = all_vals[j];
                    all_vals[j] = all_vals[j-1];
                    all_vals[j-1] = tmp;
                }

        /* Remove duplicates */
        size_t uniq = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (i == 0 || all_vals[i] != all_vals[i-1]) {
                all_vals[uniq++] = all_vals[i];
            }
        }
        all_len = uniq;
    }

    if (n == 0 || n > all_len) return 0;
    return all_vals[n-1];
}
