/*
 * A068064  a(n) = number of integers k such that palindrome A068062(n) = k +
 *          reverse(k).
 * https://oeis.org/A068064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of representations of the n-th palindrome as k +
 *            reverse(k)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   rev_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are typedefs supplied by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev_number.h"

/* Upper bound for pre‑computation of k + reverse(k) sums.
   20 000 000 comfortably covers the first 10 000 palindromes. */
#define MAXP 20000000U

/* Static storage */
static uint32_t *cnt  = NULL;          /* cnt[s] = number of k with k+rev(k)=s */
static uint64_t *pal  = NULL;          /* list of palindromes in ascending order */
static size_t pal_len = 0;
static size_t pal_cap = 0;
static bool init_done = false;

/* ---------- helper: test for palindrome ---------- */
static bool is_palindrome(idx_t x)
{
    return rev_number(x) == x;
}

/* ---------- helper: ensure capacity for pal array ---------- */
static void ensure_pal_cap(size_t needed)
{
    if (pal_cap >= needed) return;
    size_t new_cap = pal_cap ? pal_cap * 2 : 1024;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(pal, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    pal = tmp;
    pal_cap = new_cap;
}

/* ---------- one‑time initialization ---------- */
static void init(void)
{
    if (init_done) return;
    /* Allocate counts array */
    cnt = (uint32_t *)calloc((size_t)MAXP + 1, sizeof(uint32_t));
    if (!cnt) exit(EXIT_FAILURE);

    /* Count representations for every possible sum k + rev(k) */
    for (idx_t k = 0; k <= MAXP; ++k) {
        idx_t revk = rev_number(k);
        idx_t s = k + revk;
        if ((idx_t)s <= MAXP) {
            ++cnt[(size_t)s];
        }
    }

    /* Generate sorted list of representable palindromes up to MAXP */
    for (idx_t v = 0; v <= MAXP; ++v) {
        if (is_palindrome(v) && cnt[(size_t)v] > 0) {
            ensure_pal_cap(pal_len + 1);
            pal[pal_len++] = (uint64_t)v;
        }
    }

    init_done = true;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* OEIS offset is 1 */
    init();

    if ((size_t)n > pal_len) {
        /* In practice this should not happen for n ≤ 10000 with MAXP=20 000 000 */
        return 0;
    }

    uint64_t pal_val = pal[(size_t)n - 1];
    return (seq_t)cnt[(size_t)pal_val];
}
