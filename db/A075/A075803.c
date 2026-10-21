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
 * Helpers:   ensure_capacity, is_squarefree, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/seq_pow.h"
#include "../utils/is_squarefree.h"

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
        seq_t mult = seq_pow((seq_t)10, (idx_t)pref_len);
        return pref * mult + rev;
    } else {                                  /* odd length */
        seq_t rev = reverse_num(pref / ((seq_t)10));
        seq_t mult = seq_pow((seq_t)10, (idx_t)(pref_len - 1));
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
                pref_start = seq_pow((seq_t)10, (idx_t)(pref_len - 1));
            pref_end = seq_pow((seq_t)10, (idx_t)pref_len) - ((seq_t)1);
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
            ensure_capacity(diffs_len + 1, &diffs_cap, (void **)&diffs, sizeof(seq_t));
            diffs[diffs_len++] = diff;
            prev_pal = curr_pal;
        }
    }

    return diffs[(size_t)n - 1];
}
