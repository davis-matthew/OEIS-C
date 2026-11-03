/*
 * A075803  Differences between adjacent palindromic squarefree numbers.
 * https://oeis.org/A075803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = difference between successive palindromic squarefree
 *            numbers, as defined in OEIS A075803
 * Range:     u8 n <= 26, u16 n <= 549, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 86 terms through n = 966 (2026-11-03)
 * Helpers:   ipow_idx, is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* reverse a decimal number */
static seq_t reverse_num(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x = x / 10;
    }
    return rev;
}

/* build a palindrome of given length from its left half (pref) */
static seq_t make_pal(seq_t pref, int len)
{
    int pref_len = (len + 1) / 2;             /* length of the left half      */
    if (len % 2 == 0) {                       /* even length */
        seq_t rev = reverse_num(pref);
        seq_t mult = ipow_idx((seq_t)10, (idx_t)pref_len);
        return pref * mult + rev;
    } else {                                  /* odd length */
        seq_t rev = reverse_num(pref / ((seq_t)10));
        seq_t mult = ipow_idx((seq_t)10, (idx_t)(pref_len - 1));
        return pref * mult + rev;
    }
}

/* compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* Static state across calls */
    static seq_t *diffs = NULL;
    static size_t diffs_len = 0;
    static size_t diffs_cap = 0;

    static idx_t cur_len = 0;          /* current palindrome length being generated */
    static seq_t pref_start = 1;       /* first prefix for current length   */
    static seq_t pref_end = 9;         /* last  prefix for current length   */
    static seq_t pref_cur = 1;         /* next prefix to use                */
    static seq_t prev_pal = 0;         /* previous palindromic squarefree number */
    static bool prev_set = false;      /* whether prev_pal is valid          */

    /* Initialize on first invocation */
    if (diffs_len == 0) {
        cur_len = 1;
        pref_start = 1;
        pref_end = 9;
        pref_cur = 1;
        prev_set = false;
    }

    /* Generate terms until we have at least n differences */
    while ((size_t)n > diffs_len) {
        /* Advance to next length if needed */
        if (pref_cur > pref_end) {
            /* Move to next palindrome length */
            cur_len++;
            int pref_len = (cur_len + 1) / 2;
            if (pref_len == 1)
                pref_start = 1;
            else
                pref_start = ipow_idx((seq_t)10, (idx_t)(pref_len - 1));
            pref_end = ipow_idx((seq_t)10, (idx_t)pref_len) - ((seq_t)1);
            pref_cur = pref_start;
        }

        /* Build the next palindrome candidate */
        seq_t curr_pal = make_pal(pref_cur, (int)cur_len);
        pref_cur++;   /* advance prefix for next iteration */

        /* Skip non‑squarefree palindromes */
        if (!is_squarefree(curr_pal))
            continue;

        /* Store difference or initialize previous palindrome */
        if (!prev_set) {
            prev_pal = curr_pal;
            prev_set = true;
        } else {
            seq_t diff = curr_pal - prev_pal;
            /* Ensure capacity and store difference */
            ensure_capacity(diffs_len + 1, &diffs_cap, OEISC_VPP(&diffs), sizeof(seq_t));
            diffs[diffs_len++] = diff;
            prev_pal = curr_pal;
        }
    }

    return diffs[(size_t)n - 1];
}
