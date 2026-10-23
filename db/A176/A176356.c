/*
 * A176356  Numbers which when seen in a mirror are primes (or 1), using
 *          calculator-style 7-segment numerals.
 * https://oeis.org/A176356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big, mirror_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176356: numbers whose calculator‑style 7‑segment mirror is
 * prime or 1.  The function builds a cache of terms incrementally,
 * allowing queries in any order.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/mirror_of.h"

/* cache of found terms; 0‑based */
static seq_t *cache = NULL;
static idx_t cache_count = 0;   /* number of cached terms          */
static idx_t cache_cap   = 0;    /* current cache allocation size   */
static seq_t  max_checked = 0;   /* largest integer that has been examined */

/* enlarge the cache array to at least new_cap elements */
static void ensure_capacity(idx_t new_cap) {
    if (new_cap <= cache_cap) return;
    idx_t alloc = new_cap * 2;
    if (alloc < new_cap) alloc = new_cap;          /* never overflow */
    seq_t *ptr = (seq_t *)realloc(cache, alloc * sizeof(seq_t));
    cache = ptr;
    cache_cap = alloc;
}

/* return true iff n contains only allowed digits 0,1,2,5,8 */
static bool valid_digits(seq_t n) {
    if (n == 0) return false;                     /* 0 is not in the sequence */
    seq_t temp = n;
    while (temp > 0) {
        unsigned long long d = (unsigned long long)(temp % 10);
        if (d != 0 && d != 1 && d != 2 && d != 5 && d != 8)
            return false;
        temp /= 10;
    }
    return true;
}

/* generate terms until we have at least n terms cached */
static void generate_up_to(idx_t n) {
    if (n <= cache_count) return;

    seq_t cur = max_checked + 1;
    while (cache_count < n) {
        if (valid_digits(cur)) {
            seq_t mir = mirror_of(cur);
            if (mir == 1 || is_prime_big(mir)) {
                ensure_capacity(cache_count + 1);
                cache[cache_count] = cur;
                cache_count++;
            }
        }
        cur++;
    }
    max_checked = cur - 1;
}

seq_t a(idx_t n) {
    /* OEIS is 1‑based.  Return the n‑th term. */
    if (n == 0) return 0;                    /* undefined, return 0 */
    if (n > cache_count) generate_up_to(n);
    return cache[n - 1];
}
