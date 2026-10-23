/*
 * A037268  Sum of reciprocals of digits = 1.
 * https://oeis.org/A037268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 1209, u64 n <= 1209,
 *            u128 n <= 1209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037268: numbers whose sum of reciprocals of digits equals 1. */
/*  The sequence is built by backtracking over digit multisets 2–9 whose
 *  weighted sum (2520/d) equals 2520, then generating all permutations
 *  of those multisets.  The resulting numbers are sorted and deduplicated. */

#include <stdlib.h>
#include <stdint.h>

/* Weights = 2520 / digit for digits 2..9 */
static const int WEIGHTS[10] = {
    0,          /* 0 unused */
    0,          /* 1 unused – only the single digit 1 is allowed */
    1260,       /* 2520/2 */
    840,        /* 2520/3 */
    630,        /* 2520/4 */
    504,        /* 2520/5 */
    420,        /* 2520/6 */
    360,        /* 2520/7 */
    315,        /* 2520/8 */
    280          /* 2520/9 */
};

/* Global storage for the sequence.  The array is constructed once. */
static seq_t *seq = NULL;      /* array of terms */
static size_t seq_len = 0;     /* number of terms stored */
static size_t seq_cap = 0;     /* capacity of the array */

/* Add a value to the dynamic array, growing if necessary. */
static void add_value(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq[seq_len++] = v;
}

/* next_permutation for an array of ints, returns 1 if a next permutation
   exists, otherwise 0.  The array must be sorted non‑decreasing before
   the first call. */
static int next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return 0;
    int j = n - 1;
    while (a[j] <= a[i]) --j;
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        ++l;
        --r;
    }
    return 1;
}

/* Generate all numeric permutations of the multiset stored in `digits`
   (of length `len`) and add them to the global array. */
static void permute_and_store(int *digits, int len)
{
    int perm[10];
    for (int i = 0; i < len; ++i) perm[i] = digits[i];
    do {
        seq_t v = 0;
        for (int i = 0; i < len; ++i) {
            v = v * 10 + (seq_t)perm[i];
        }
        add_value(v);
    } while (next_permutation(perm, len));
}

/* Backtracking to find all multisets of digits 2–9 whose weighted sum
   of reciprocals equals 1 (i.e., total weight 2520). */
static void backtrack(int start_digit, int remaining, int *digits, int len)
{
    if (remaining == 0) {
        permute_and_store(digits, len);
        return;
    }
    if (remaining < 0) return;
    for (int d = start_digit; d <= 9; ++d) {
        int w = WEIGHTS[d];
        if (w == 0) continue;
        if (w > remaining) continue;
        digits[len++] = d;
        backtrack(d, remaining - w, digits, len);
        --len;
    }
}

/* Comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a, *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Build the complete sequence once, sorted and deduplicated. */
static void build_sequence(void)
{
    if (seq != NULL) return;   /* already built */

    /* include the single digit 1 */
    add_value((seq_t)1);

    int digits[10];
    backtrack(2, 2520, digits, 0);

    /* sort the array */
    qsort(seq, seq_len, sizeof(seq_t), cmp_seq);

    /* remove duplicates */
    size_t uniq = 0;
    for (size_t i = 0; i < seq_len; ++i) {
        if (uniq == 0 || seq[i] != seq[uniq - 1]) {
            seq[uniq++] = seq[i];
        }
    }
    seq_len = uniq;
}

/* ------------------------------------------------------------------ */
/* Public API required by the build system                        */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; undefined for 0 */
    build_sequence();
    if (n <= (idx_t)seq_len) {
        return seq[n - 1];  /* 1‑based indexing */
    }
    /* if n exceeds known terms, sequence is not defined; return 0 */
    return (seq_t)0;
}
