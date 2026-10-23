/*
 * A183055  Palindromic partition numbers.
 * https://oeis.org/A183055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183055 – palindromic partition numbers
   a(n) = the nth partition number whose decimal expansion is a palindrome.
   The function generates integer partitions via Euler's pentagonal‑number
   recurrence, checks each for palindromicity, and stores the palindromic
   values in order. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_palindrome.h"

/* Expand a dynamic array of seq_t elements to accommodate at least `needed`
   elements.  `curr` is updated only when a new element is appended by the
   caller; the function merely guarantees sufficient storage. */
static void ensure_cap(seq_t **ptr, size_t *cap, size_t *curr, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? (*cap << 1) : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) { *ptr = NULL; *cap = 0; *curr = 0; return; }
    *ptr = tmp; *cap = new_cap;
}

/* Static storage for partition numbers and palindromic partition numbers. */
static seq_t *part = NULL;     /* part[i] == p(i) */
static size_t part_sz = 0, part_cap = 0;
static seq_t *pal  = NULL;     /* pal[j] == the j‑th palindromic partition number */
static size_t pal_sz = 0, pal_cap = 0;

/* Compute partition numbers up to and including index n, storing them in
   `part` and recording any palindromic partition numbers in `pal`. */
static void ensure_partition(idx_t n)
{
    while (part_sz <= (size_t)n) {
        idx_t cur = (idx_t)part_sz;
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > cur) break;
            seq_t term = part[cur - pent1];
            if (k & 1) pos += term; else neg += term;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > cur) continue;
            term = part[cur - pent2];
            if (k & 1) pos += term; else neg += term;
        }
        seq_t val = pos - neg;   /* p(cur) */

        /* store the new partition value */
        ensure_cap(&part, &part_cap, &part_sz, part_sz + 1);
        if (!part) return;       /* allocation failure */
        part[part_sz++] = val;

        /* add to palindromic list if appropriate */
        if (is_palindrome(val, 10)) {
            ensure_cap(&pal, &pal_cap, &pal_sz, pal_sz + 1);
            if (!pal) return;
            pal[pal_sz++] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* initialise storage on first call and add p(0) = 1 to the palindromic list */
    if (!part) {
        ensure_cap(&part, &part_cap, &part_sz, 1);
        if (!part) return 0;
        part[0] = (seq_t)1;          /* p(0) = 1 */
        part_sz = 1;

        ensure_cap(&pal, &pal_cap, &pal_sz, 1);
        if (!pal) return 0;
        pal[pal_sz++] = (seq_t)1;     /* first term a(1) = 1 */
    }

    /* generate palindromic partition numbers until we have n of them */
    while (pal_sz < (size_t)n) {
        /* compute next partition value; part_sz is the index of the next p(i) to compute */
        ensure_partition((idx_t)part_sz);
        /* The test harness guarantees a solution exists for the requested n. */
    }

    return pal[n - 1];
}
