/*
 * A044527  Numbers n such that string 1,1 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose base‑7 representation
   contains the substring "11" while the representation of n+1 does not.
   Implementation uses a brute‑force scan with memoisation, good for
   n up to about 10 000. The offset is 1: a(1) is the first term (8). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache to store already‑computed terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cachecap   = 0;      /* capacity of the array */
static size_t cachelen   = 0;      /* number of terms currently stored */
static seq_t last_checked = 0;     /* last integer examined during search */

/* Return true if the base‑7 representation of x contains the substring
   "11".  Works for all seq_t integer types. */
static bool has11(seq_t x)
{
    if (x == 0) return false;

    int prev_digit = -1;            /* no previous digit yet */
    seq_t tmp = x;
    int cur_digit = (int)(tmp % 7);
    tmp /= 7;
    prev_digit = cur_digit;

    while (tmp > 0) {
        cur_digit = (int)(tmp % 7);
        if (cur_digit == 1 && prev_digit == 1)
            return true;
        prev_digit = cur_digit;
        tmp /= 7;
    }
    return false;
}

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, unused because offset = 1 */

    /* If already computed, return cached value. */
    if (n <= cachelen) return cache[n - 1];

    /* Extend cache until we have enough terms. */
    seq_t cur = last_checked + 1;
    while (cachelen < n) {
        if (has11(cur) && !has11(cur + 1)) {
            /* Ensure enough space for the new term. */
            if (cachelen == cachecap) {
                size_t newcap = cachecap ? cachecap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache     = tmp;
                cachecap  = newcap;
            }
            cache[cachelen++] = cur;
        }
        cur++;
    }
    last_checked = cur - 1;
    return cache[n - 1];
}
