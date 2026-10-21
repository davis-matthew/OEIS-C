/*
 * A050420  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a palindrome (possibly beginning with 0) is 1.
 * https://oeis.org/A050420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 4622, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A050420
 *  Numbers which are not binary palindromes but become a palindrome after
 *  deleting exactly one binary digit (leading zeros after deletion are
 *  allowed).  The function searches for candidates sequentially and caches
 *  them, so calls to a(n) need not be monotonic.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- small helpers --------------------------------------------------    */
/* return the number of bits required to represent x (x > 0).               */
static int bitlen_seq_t(seq_t x)
{
    int l = 0;
    while (x != 0) {
        l++;
        x >>= 1;
    }
    return (l > 0) ? l : 1;   /* x will never be zero in this context   */
}

/* test whether x is a binary palindrome.                                 */
static bool is_bin_pal(seq_t x)
{
    int len = bitlen_seq_t(x);
    for (int i = 0; i < len / 2; ++i) {
        uint64_t mask_hi = (uint64_t)1 << (len - 1 - i);
        uint64_t mask_lo = (uint64_t)1 << i;
        if (((x & mask_hi) != 0) != ((x & mask_lo) != 0))
            return false;
    }
    return true;
}

/* return true if removing exactly one bit from x gives a binary palindrome */
static bool qualifies(seq_t x)
{
    if (x == 0)                /* numbers start from 1 */
        return false;

    if (is_bin_pal(x))         /* already a palindrome ⇒ need >1 removal   */
        return false;

    int len = bitlen_seq_t(x);
    /* try removing each bit position k (0 is most significant) */
    for (int k = 0; k < len; ++k) {
        int i = 0, j = len - 1;
        bool ok = true;
        while (i < j) {
            if (i == k) { ++i; continue; }
            if (j == k) { --j; continue; }
            uint64_t mask_i = (uint64_t)1 << (len - 1 - i);
            uint64_t mask_j = (uint64_t)1 << (len - 1 - j);
            if (((x & mask_i) != 0) != ((x & mask_j) != 0)) {
                ok = false;
                break;
            }
            ++i; --j;
        }
        if (ok)            /* found a single‑bit deletion yielding a palindrome */
            return true;
    }
    return false;
}

/* --- main function ----------------------------------------------------- */
/* cache[0] is unused; cache[1] is the first term. */
seq_t a(idx_t n)
{
    /* static data for caching results */
    static seq_t *cache = NULL;   /* cache[0] unused */
    static size_t cache_size = 0; /* number of stored terms (including index 0) */
    static size_t cache_capacity = 0;
    static seq_t last_checked = 0; /* last number examined */

    if (n == 0)                   /* the sequence is defined for n ≥ 1      */
        return 0;

    /* initialise cache on first entry */
    if (cache == NULL) {
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) abort();            /* out of memory (shouldn't happen) */
        cache[0] = 0;                           /* unused placeholder             */
        cache_size = 1;                         /* currently only the 0th entry   */
    }

    /* grow cache if necessary */
    if (cache_size <= n) {
        size_t new_capacity = cache_capacity;
        while (new_capacity <= n) new_capacity <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* continue generating candidates until we have n terms. */
    while (cache_size <= n) {
        ++last_checked;
        if (qualifies(last_checked))
            cache[cache_size++] = last_checked;
    }

    return cache[n];
}
