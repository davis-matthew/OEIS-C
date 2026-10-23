/*
 * A032964  Numbers whose base-14 representation Sum_{i=0..m} d(i)*14^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   allocate_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑14 representation has even digits in every odd position
   (OEIS A032964).  The property is checked by iterating over the base‑14
   digits from the most significant side and requiring that digits in odd
   indices (0‑based) are even. */
#include <stdbool.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"

/* Check whether all digits at odd positions (counting from the most
   significant digit as index 0) of n are even in base 14. */
static bool good_base14(seq_t n)
{
    /* Extract base‑14 digits into an array (least significant first). */
    unsigned char digits[64];          /* enough for 128‑bit numbers */
    size_t len = 0;
    seq_t t = n;
    if (t == 0) {
        digits[len++] = 0;
    } else {
        while (t > 0) {
            digits[len++] = (unsigned)(t % 14);
            t /= 14;
        }
    }

    /* Scan digits from the most significant side.  The index of a
       digit from the most significant side is: idx = len-1-k where
       k is the position from the least significant side.  We
       require that digits with odd indices be even. */
    for (size_t pos = 1; pos < len; pos += 2) {
        if (digits[len - 1 - pos] & 1)   /* odd digit -> fail */
            return false;
    }
    return true;
}

/* Cache the terms as they are discovered.  The cache grows
   only when a larger n is requested. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;      /* number of terms stored */
static seq_t curr = 1;            /* candidate to test next */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* Allocate or grow the cache if necessary.  The helper
       seq_realloc copies existing data when expanding. */
    if (n > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)n);
        cache_cap = (size_t)n;
    }

    /* Fill the cache up to the requested term. */
    while (cache_len < (size_t)n) {
        if (good_base14(curr))
            cache[cache_len++] = curr;
        ++curr;
    }

    return cache[n - 1];
}
