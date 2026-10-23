/*
 * A044145  Numbers k such that string 1,0 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1234, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Sequence A044145: a(n) is the n-th integer k such that the string “10” occurs
   in the base‑7 representation of k but not in the representation of k‑1.  The
   function returns a(1)=7, a(2)=49, etc.  It is computed on demand and cached,
   so calls may be in any order. */

static bool has10(seq_t value) {
    if (value == 0) return false;
    unsigned char digits[64];
    size_t len = 0;
    seq_t temp = value;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % 7);
        temp /= 7;
    }
    for (size_t i = len; i > 1; --i) {
        if (digits[i - 1] == 1 && digits[i - 2] == 0)
            return true;
    }
    return false;
}

static bool good_k(seq_t k) {
    if (!has10(k)) return false;
    seq_t km1 = k - 1;
    return !has10(km1);
}

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

static void grow_cache(void) {
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, return 0 for safety */
    while (cache_size < n) {
        last_checked++;
        if (good_k(last_checked)) {
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = last_checked;
        }
    }
    return cache[n - 1];
}
