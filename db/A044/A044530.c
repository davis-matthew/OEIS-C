/*
 * A044530  Numbers n such that string 1,4 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer n such that its base‑7
 *            representation contains the substring "14" while the base‑7
 *            representation of n+1 does NOT contain "14"
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool has14(seq_t x)
{
    if (x == 0) return false;          /* "0" contains no "14" */
    seq_t t = x;
    int digits[64];
    int len = 0;
    while (t > 0) {
        digits[len++] = (int)(t % 7);
        t /= 7;
    }
    int prev = -1;
    for (int i = len - 1; i >= 0; --i) {
        int cur = digits[i];
        if (prev == 1 && cur == 4) return true;
        prev = cur;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* cache[0] == a(1) */
    static size_t cap = 0, sz = 0;
    static seq_t next_candidate = 1;   /* next number to test */

    if (n == 0) return 0;             /* offset is 1, so undefined for 0 */

    /* extend the cache until it contains n elements */
    while (sz < n) {
        if (has14(next_candidate) && !has14(next_candidate + 1)) {
            if (sz == cap) {
                cap = cap ? cap * 2 : 10;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
