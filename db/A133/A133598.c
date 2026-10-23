/*
 * A133598  Numbers k with all digits distinct and nonzero, such that none of
 *          k's digits divide k, but all the nonzero digits not in k do divide
 *          k.
 * https://oeis.org/A133598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9140, u64 n <= 9140, u128 n <= 9140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A133598:
   Numbers k with all digits distinct and non‑zero, such that none of k's digits divide k,
   but all non‑zero digits not in k do divide k.
   The function generates all permutations of subsets of {1,…,9}, tests the
   divisibility condition, sorts the valid numbers and returns the nth smallest
   (1‑based). */

/* Necessary includes – valid for both C11 and C++17 */
#include <stdlib.h>
#include <stdbool.h>

/* Helper: swap two seq_t values */
static void swap_seq(seq_t *a, seq_t *b) {
    seq_t t = *a; *a = *b; *b = t;
}

/* Quick‑sort implementation for seq_t (works for both C and C++). */
static void quicksort(seq_t *arr, int lo, int hi) {
    if (lo >= hi) return;
    int i = lo, j = hi;
    seq_t pivot = arr[(lo + hi) / 2];
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) { swap_seq(&arr[i], &arr[j]); i++; j--; }
    }
    if (lo < j)  quicksort(arr, lo, j);
    if (i < hi)  quicksort(arr, i, hi);
}

/* Test whether a candidate number satisfies the sequence condition. */
static bool is_valid(seq_t k, uint16_t mask) {
    for (int d = 1; d <= 9; ++d) {
        bool present = (mask & (1u << d)) != 0;
        if (present) {
            if (k % (seq_t)d == 0) return false;
        } else {
            if (k % (seq_t)d != 0) return false;
        }
    }
    return true;
}

/* Recursive generation of all numbers using distinct non‑zero digits. */
static void generate_rec(seq_t value, uint16_t mask,
                         seq_t *buf, size_t *cnt, size_t cap) {
    if (value != 0) {
        if (is_valid(value, mask)) {
            if (*cnt < cap) buf[(*cnt)++] = value;
        }
    }
    if (mask == 0x3fe) return;                           /* digits 1‑9 all used */
    for (int d = 1; d <= 9; ++d) {
        if (!(mask & (1u << d))) {
            generate_rec(value * 10 + d, mask | (1u << d),
                         buf, cnt, cap);
        }
    }
}

/* Static storage for the computed sequence */
static seq_t *seq_vals   = NULL;
static size_t seq_len    = 0;
static bool   seq_built  = false;

/* Build the sequence on first call */
static void build_sequence(void) {
    /* Number of all permutations of subsets of digits 1–9: Σ P(9,k) = 986409 */
    const size_t MAX_CAND = 986409;
    seq_vals = (seq_t *)malloc(MAX_CAND * sizeof(seq_t));
    seq_len = 0;
    generate_rec(0, 0, seq_vals, &seq_len, MAX_CAND);
    if (seq_len > 0) quicksort(seq_vals, 0, (int)seq_len - 1);
    seq_built = true;
}

/* External linkage – required function for the OEIS evaluator */
seq_t a(idx_t n) {
    if (!seq_built) build_sequence();
    if (n == 0 || (idx_t)seq_len < n) return 0; /* out of bounds */
    return seq_vals[(size_t)(n - 1)];
}
