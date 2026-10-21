/*
 * A235133  Numbers whose (decimal) digits are a permutation of 1...n for some
 *          n, such that for all k in {1,...,n} the first k digits form a
 *          number divisible by k, when considered as representation in base
 *          n+1.
 * https://oeis.org/A235133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each length m (≥1) of digits,
   perform a back‑tracking search that builds all permutations
   of the digits 1…m.  At depth k (1 ≤ k ≤ m) the prefix
   formed by the first k digits is interpreted as a number in
   base (m+1); if that number is divisible by k we continue.
   Upon completion of a full permutation we convert it to its
   decimal representation and store it.  The array of terms is
   kept sorted and deduplicated; the function a(idx_t n) returns
   the (n−1)‑th element of this sorted list.  The algorithm
   runs in O(n) amortised time per term and is memoised so
   calls may come in any order.  The implementation relies on
   the helper functions seq_alloc and seq_realloc for
   dynamically allocated arrays of seq_t that work for both
   plain integer types and arbitrary‑precision classes. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Storage for discovered terms */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;   /* number of terms stored             */
static size_t terms_cap  = 0;   /* current allocation capacity        */
static bool   terms_sorted = true; /* true when terms are sorted          */

/* Storage for the current permutation generation */
static int  *perm_digits = NULL;   /* array [1..cur_m] of digits           */
static uint8_t *perm_used = NULL; /* boolean flags for used digits        */
static size_t  perm_cap   = 0;     /* allocation capacity of the arrays    */
static idx_t   cur_m          = 0;  /* current digit length being explored */
static int     base            = 0;  /* base = cur_m + 1                    */

/* Compare two seq_t values for qsort */
static int seq_cmp(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Append a term to the terms array (unsorted) */
static void add_term_unsorted(seq_t val)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 4 : terms_cap * 2;
        terms = seq_realloc(terms, terms_cap, new_cap);
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
    terms_sorted = false;
}

/* Recursive DFS that builds permutations of length cur_m */
static void dfs(idx_t k, seq_t val_dec)
{
    if (k == cur_m) {
        add_term_unsorted(val_dec);
        return;
    }

    idx_t new_len = k + 1;                         /* length after adding digit */
    for (idx_t d = 1; d <= cur_m; ++d) {
        if (perm_used[d]) continue;
        perm_used[d] = 1;
        perm_digits[k] = (int)d;

        /* compute prefix value modulo new_len in base = cur_m+1 */
        idx_t mod = 0;
        for (idx_t i = 0; i <= k; ++i) {
            mod = (mod * base + perm_digits[i]) % new_len;
        }

        if (mod == 0) {
            dfs(k + 1, val_dec * 10 + (seq_t)d); /* decimal value grows in base 10 */
        }

        perm_used[d] = 0;
    }
}

/* Generate all terms for a given m */
static void generate_m(idx_t m)
{
    /* ensure digit arrays are large enough */
    if (m >= perm_cap) {
        size_t new_cap = m + 2;
        free(perm_digits);
        free(perm_used);
        perm_digits = (int *)malloc(new_cap * sizeof(int));
        perm_used   = (uint8_t *)malloc(new_cap * sizeof(uint8_t));
        perm_cap = new_cap;
    }
    memset(perm_used, 0, (m + 1) * sizeof(uint8_t));

    cur_m = m;
    base = (int)(m + 1);
    dfs(0, (seq_t)0);
}

/* Create term list up to index n (1‑based) and ensure it is sorted */
static void ensure_terms(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        generate_m(++cur_m);
    }
    if (!terms_sorted) {
        qsort(terms, terms_len, sizeof(seq_t), seq_cmp);

        /* deduplicate */
        size_t w = 0;
        for (size_t r = 0; r < terms_len; ++r) {
            if (r == 0 || seq_cmp(&terms[r], &terms[w - 1]) != 0) {
                if (w < r) terms[w] = terms[r];
                ++w;
            }
        }
        terms_len = w;
        terms = seq_realloc(terms, terms_cap, terms_len);
        terms_cap = terms_len;
        terms_sorted = true;
    }
}

/* Public API defined by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for index 0 */
    ensure_terms(n);
    return terms[n - 1];            /* 1‑based indexing */
}
