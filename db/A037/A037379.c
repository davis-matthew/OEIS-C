/*
 * A037379  Positive numbers k such that every base-2 digit of k is a base-10
 *          digit of k.
 * https://oeis.org/A037379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 8831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth positive integer k such that each binary
   digit of k is one of its decimal digits.  The implementation
   tests each candidate sequentially until the requested number of
   terms have been found. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_digits.h"

/* Return true if k satisfies the defining property. */
static bool ok(seq_t k)
{
    /* Extract decimal digits of k. */
    uint8_t buf[40];
    size_t len;
    get_digits(k, buf, &len);

    bool has0 = false, has1 = false;
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] == 0) has0 = true;
        if (buf[i] == 1) has1 = true;
        if (has0 && has1) break;
    }
    if (!has1) return false;            /* 1 must be a decimal digit */

    /* Does the binary form of k contain a zero bit? */
    bool hasZeroBit = ((k & (k + (seq_t)1)) != (seq_t)0);

    /* Need a 0 in decimal iff the binary representation has a 0. */
    return !hasZeroBit || has0;
}

/* Public function returning the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of found terms and the next candidate to test. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t next   = 1;          /* candidate numbers start at 1 */

    /* Initialise cache lazily. */
    if (len == 0 && n > 0) {
        cap   = 16;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        if (ok(next)) {
            if (len == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = next;
        }
        ++next;
    }

    return cache[n - 1];
}
