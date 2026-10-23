/*
 * A044534  Numbers n such that string 2,1 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence consists of integers n such that the base‑7 representation of
 * n contains the substring "21" but the base‑7 representation of n+1 does
 * not.  The function below computes the nth term by scanning integers in
 * order and caching the ones that meet the criterion.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* return true if x in base‑7 contains the substring 21 */
static bool has21(seq_t x)
{
    if (x == 0) return false;
    int digits[25];
    int len = 0;
    while (x != 0) {
        digits[len++] = (int)(x % 7);
        x /= 7;
    }
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 2 && digits[i - 1] == 1)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 – index 0 is unused */

    /* static cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t current = 1;   /* next integer to test */

    while (cache_len < n) {
        seq_t m = current;
        if (has21(m) && !has21(m + 1)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* memory allocation failed */
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        ++current;
    }

    return cache[n - 1];
}
