/*
 * A179697  Base-10 representation of numbers k which, in base 2, satisfy
 *          abs(k + reverse(k) - reverse(k + reverse(k))) = abs(k -
 *          reverse(k)) + reverse(abs(k - reverse(k))) = k.
 * https://oeis.org/A179697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179697 in base‑10:
   a(n) = 2p_n
   where p_n is the n‑th binary palindrome for which 
   rev(3·p_n) = p_n  or  rev(3·p_n) = 5·p_n   (rev works in base 2,
   stripping leading zeros).  This condition is equivalent to the
   two equalities stated in the OEIS entry.  */
/* The function below generates binary palindromes in numeric order
   and keeps those satisfying the above property.  The result is
   memoised in a static array so that any calls with arbitrary n
   are answered in O(count_of_needed_terms). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

static seq_t rev_seq(seq_t x)
{
    seq_t rev = 0;
    size_t len = bitlen_seq_t(x);
    for (size_t i = 0; i < len; ++i)
    {
        rev = (rev << 1) | (x & (seq_t)1);
        x >>= 1;
    }
    return rev;
}

/* Build the binary palindrome of length 'len' from the bit pattern 'left'.
   'left' contains the first half (including the middle bit if len is odd),
   with the most significant bit forced to 1. */
static seq_t build_pal(size_t len, uint64_t left)
{
    seq_t p = 0;
    size_t half = (len + 1) / 2;
    /* append the left half (most significant bits first) */
    for (int i = (int)half - 1; i >= 0; --i)
        p = (p << 1) | ((left >> i) & 1ULL);
    /* append the mirrored part */
    if (len % 2 == 0)               /* even length: mirror all bits */
    {
        for (int i = 0; i < (int)half; ++i)
            p = (p << 1) | ((left >> i) & 1ULL);
    }
    else                            /* odd length: skip the middle bit on mirroring */
    {
        for (int i = 1; i < (int)half; ++i)
            p = (p << 1) | ((left >> i) & 1ULL);
    }
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, but safe to return 0 */

    /* --- memoisation container -------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;

    /* --- palindrome generator state -------------------------------- */
    static size_t pal_len  = 1;         /* current palindrome length in bits */
    static size_t half_len = 1;         /* ceil(pal_len/2)   */
    static uint64_t left_start = 1;     /* smallest left half */
    static uint64_t left_end   = 2;     /* one past largest left half */
    static uint64_t left_cur   = 1;     /* current left half to use */

    /* initialise generator state on the first call */
    static int init = 1;
    if (init)
    {
        init = 0;
        pal_len   = 1;
        half_len  = 1;
        left_start = 1ULL << (half_len - 1);
        left_end   = 1ULL << half_len;
        left_cur   = left_start;
    }

    while (cache_sz < (size_t)n)
    {
        /* --- produce next binary palindrome ------------------------ */
        seq_t p = build_pal(pal_len, left_cur);

        /* move to the next left half */
        ++left_cur;
        if (left_cur == left_end)
        {
            ++pal_len;
            half_len = (pal_len + 1) / 2;
            left_start = 1ULL << (half_len - 1);
            left_end   = 1ULL << half_len;
            left_cur   = left_start;
        }

        /* --- test the defining property for p -------------------- */
        seq_t three_p   = p * (seq_t)3;
        seq_t rev_three = rev_seq(three_p);

        if (rev_three == p || rev_three == p * (seq_t)5)
        {
            /* store k = 2 * p */
            seq_t k = p << 1;

            if (cache_sz == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = k;
        }
    }
    return cache[n - 1];
}
