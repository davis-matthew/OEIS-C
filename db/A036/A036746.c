/*
 * A036746  Numbers with "long" representations in Roman notation: given by
 *          last n letters from ...MMMDCCCLXXXVIII.
 * https://oeis.org/A036746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min{k>0 | A006968(k)=n} * Computes the least integer
 *            whose Roman numeral representation uses n digits. * The
 *            implementation iteratively builds Roman‑lengths of consecutive
 *            integers * and stores the first occurrence of each length
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned char ones_len[10]   = {0,1,2,3,2,1,2,3,4,2};
static const unsigned char tens_len[10]   = {0,1,2,3,2,1,2,3,4,2};
static const unsigned char hundreds_len[10]= {0,1,2,3,2,1,2,3,4,2};

static size_t roman_len(idx_t num)
{
    size_t len = num / 1000;                 /* M's */
    idx_t  h = (num / 100) % 10;             /* hundreds digit */
    idx_t  t = (num / 10) % 10;              /* tens digit */
    idx_t  o =  num        % 10;             /* ones digit */
    len += hundreds_len[h];
    len += tens_len[t];
    len += ones_len[o];
    return len;
}

static void ensure_capacity(size_t required, seq_t **arr, size_t *cap)
{
    if (required < *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap <= required) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!new_arr) return; /* allocation failed; caller will get 0 */
    for (size_t i = *cap; i < new_cap; ++i) new_arr[i] = 0;
    *arr = new_arr;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static state across calls */
    static seq_t *memo = NULL;      /* memo[len] = minimal k with Roman length=len */
    static size_t memo_cap = 0;
    static idx_t  next_k = 1;        /* next integer to test */
    static idx_t  max_len = 0;       /* greatest length discovered so far */

    /* Compute until we have covered length n */
    while (max_len < n) {
        seq_t cur = (seq_t)next_k;
        size_t len = roman_len(next_k);
        ensure_capacity(len, &memo, &memo_cap);
        if (memo[len] == 0) memo[len] = cur;  /* first occurrence of this length */
        if (len > max_len) max_len = (idx_t)len;
        ++next_k;
    }

    return memo[n];
}
