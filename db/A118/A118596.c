/*
 * A118596  Palindromes in base 5 (written in base 5).
 * https://oeis.org/A118596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS A118596: palindromes in base‑5 written in decimal.
   It enumerates palindromes in base 5, interprets their digits as decimal
   digits, and returns the nth such number (offset 1). */

#include <stddef.h>
#include <stdlib.h>

static seq_t build_palindrome(seq_t leftHalf, idx_t len)
{
    idx_t halfLen = (len + 1) / 2;
    seq_t digits[20];         /* enough for lengths up to 40 ( > 10000 terms ) */
    seq_t tmp = leftHalf;

    /* Extract digits of the left half, most significant first */
    for (idx_t i = 0; i < halfLen; ++i) {
        digits[halfLen - 1 - i] = tmp % ((seq_t)5);
        tmp /= ((seq_t)5);
    }

    seq_t val = (seq_t)0;
    /* First half (including middle, if odd) – interpret as decimal digits */
    for (idx_t i = 0; i < halfLen; ++i)
        val = val * ((seq_t)10) + digits[i];

    /* Second half – reflect the left half, skipping the middle digit for odd length */
    int start = (len & 1) ? (int)halfLen - 2 : (int)halfLen - 1;
    for (int i = start; i >= 0; --i)
        val = val * ((seq_t)10) + digits[i];

    return val;
}

seq_t a(idx_t n)
{
    /* Static cache for memoised results */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;

    /* Generation state */
    static idx_t curLen = 1;            /* current total length of palindrome */
    static seq_t curHalf = 0;           /* current left‑half value */
    static seq_t halfLower = 0;         /* lower bound for left‑half */
    static seq_t halfUpper = 0;         /* upper bound for left‑half */
    static idx_t halfLen = 1;           /* length of left half */
    static seq_t pow5[21];              /* powers of 5 up to 5^20 */
    static int pow5_init = 0;           /* initialisation flag */

    /* Initialise static data on first call */
    if (!pow5_init) {
        pow5[0] = (seq_t)1;
        for (int i = 1; i < 21; ++i)
            pow5[i] = pow5[i-1] * ((seq_t)5);

        halfLen   = 1;
        halfLower = 0;                     /* length 1 allows digit 0 */
        halfUpper = pow5[1] - ((seq_t)1);  /* 0–4 */
        curHalf   = halfLower;
        pow5_init = 1;
    }

    /* Allocate cache on first use */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n-1];

    /* Generate palindromes until we have at least n terms */
    while (cache_len < n) {
        seq_t pal = build_palindrome(curHalf, curLen);

        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = pal;

        /* Advance to the next left‑half value */
        if (curHalf < halfUpper) {
            ++curHalf;
        } else {
            /* Move to the next length */
            ++curLen;
            halfLen   = (curLen + 1) / 2;
            halfLower = (curLen == 1) ? 0 : pow5[halfLen-1];
            halfUpper = pow5[halfLen] - ((seq_t)1);
            curHalf   = halfLower;
        }
    }

    return cache[n-1];
}
