/*
 * A210017  In base 6, numbers n which have 6 distinct digits, do not start
 *          with 0, and have property that the product (written in base 6) of
 *          any two adjacent digits is a substring of n.
 * https://oeis.org/A210017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   check_prop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with all 6 base‑6 digits distinct, not starting with 0,
   such that for each pair of adjacent digits, the product (written in base‑6)
   appears as a contiguous substring of the 6‑digit base‑6 representation.
   Computed by exhaustive search of the 5·5! = 600 permutations. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/check_prop.h"

static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
        l++; r--;
    }
    return true;
}

static void build_terms(seq_t *terms, size_t *cnt)
{
    int digits[6] = {0, 1, 2, 3, 4, 5};
    do {
        if (digits[0] == 0) continue;
        if (!check_prop(digits)) continue;
        seq_t val = 0;
        for (int i = 0; i < 6; i++) {
            val = val * 10 + digits[i];
        }
        terms[(*cnt)++] = val;
    } while (next_permutation(digits, 6));

    /* sort ascending */
    for (size_t i = 0; i + 1 < *cnt; i++) {
        for (size_t j = i + 1; j < *cnt; j++) {
            if (terms[j] < terms[i]) {
                seq_t tmp = terms[i];
                terms[i] = terms[j];
                terms[j] = tmp;
            }
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t terms[600];
    static size_t term_count = 0;
    if (!initialized) {
        build_terms(terms, &term_count);
        initialized = true;
    }
    if (n == 0) return 0;
    if ((size_t)(n - 1) < term_count) {
        return terms[n - 1];
    }
    return 0; /* undefined for indices beyond known terms */
}
