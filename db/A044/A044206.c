/*
 * A044206  Numbers n such that string 2,3 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044206: integers n whose base‑8 representation contains
   the substring "23" but that of n−1 does not.
   The function returns the n‑th term (n ≥ 1).  For n = 0 it returns 0. */
#include <stdbool.h>
#include <stdlib.h>

/* Test whether the octal (base‑8) representation of x contains the
   consecutive digits 2 followed by 3 (most‑significant → least‑significant). */
static bool has23(seq_t x)
{
    if (x == 0) return false;          /* 0 has no digits */
    uint8_t digits[256];               /* enough for 128‑bit values */
    size_t len = 0;
    while (x != 0) {
        digits[len++] = (uint8_t)(x % 8);
        x = x / 8;
    }
    /* digits[0] is LSD, digits[len-1] is MSD.
       Scan from MSD towards LSD. */
    for (size_t i = len; i-- > 1; ) {
        if (digits[i] == 2 && digits[i - 1] == 3) return true;
    }
    return false;
}

/* Cache of already found terms to allow arbitrary call order. */
static seq_t *cache   = NULL; /** array of found terms */
static idx_t  cap     = 0;    /** capacity of cache */
static idx_t  length  = 0;    /** number of terms stored */
static seq_t  next_candidate = 1;  /* next number to test */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(idx_t need)
{
    if (need <= cap) return;
    size_t new_cap = cap ? cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* below offset – behaviour not required */
    while (length < n) {
        /* Find the next candidate satisfying the condition. */
        while (true) {
            if (has23(next_candidate) && !has23(next_candidate - 1)) {
                ensure_cache(length + 1);
                cache[length++] = next_candidate;
                /* Move past the candidate for subsequent searches. */
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
