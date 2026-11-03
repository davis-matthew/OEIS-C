/*
 * A044915  Numbers whose base-14 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth number whose base‑14 run lengths alternate odd,
 *            even, odd, … starting with odd
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff v in base‑14 has alternating run lengths
   odd, even, odd, … (first run odd). */
static bool base14_alternate_parity(seq_t v)
{
    /* v must be positive; 0 is not part of the sequence */
    if (v == 0) return false;

    /* Digit buffer – base‑14 never exceeds 10^10 for n <= 10000,
       so 200 bytes is far more than enough. */
    unsigned char digits[200];
    size_t len = 0;
    seq_t tmp = v;

    /* Extract digits least‑significant first */
    do {
        digits[len++] = (unsigned char)(tmp % (seq_t)14);
        tmp /= (seq_t)14;
    } while (tmp);

    /* Scan runs from most‑significant to least‑significant */
    bool expect_odd = true;          /* first run must be odd length */
    size_t i = len;
    while (i > 0) {
        unsigned char cur = digits[--i];   /* next digit to process   */
        size_t runlen = 1;
        /* consume identical digits in this run */
        while (i > 0 && digits[i-1] == cur) {
            --i;
            ++runlen;
        }
        /* Check run parity against expectation */
        if (expect_odd != (runlen & 1U))
            return false;
        expect_odd = !expect_odd;       /* next run should be opposite parity */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Static cache of found terms */
    static seq_t *cache     = NULL;   /* array of sequence values     */
    static size_t size      = 0;      /* number of cached elements    */
    static size_t capacity  = 0;      /* allocated capacity           */
    static seq_t next_candidate = (seq_t)1; /* next integer to test   */

    /* Ensure enough space in cache */
    if (size == capacity) {
        capacity = capacity ? capacity << 1 : 4;
        cache = mem_resize(seq_t, cache, capacity);
    }

    /* Generate terms until we have at least n terms cached */
    while (size < n) {
        if (base14_alternate_parity(next_candidate)) {
            cache[size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
