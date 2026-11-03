/*
 * A079466  Numbers k such that the "inventory" A063850 of k is a palindrome.
 * https://oeis.org/A079466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079466: numbers k whose inventory A063850(k) is a palindrome.
   Computation: scan integers sequentially, testing each one.  The routine
   keeps a cache of found terms so that arbitrary order of calls is
   supported.  All computations use only integer arithmetic on seq_t. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether the inventory of k is a palindrome.  k is of type seq_t. */
static bool inventory_is_pal(seq_t k)
{
    /* Count decimal digits of k.  The count array holds up to 10 digits. */
    unsigned cnt[10] = {0};

    if (k == (seq_t)0) {
        /* Inventory of 0 is 10 ("one 0") – not a palindrome. */
        cnt[0] = 1;
    } else {
        seq_t tmp = k;
        while (tmp > (seq_t)0) {
            unsigned d = (unsigned)(tmp % (seq_t)10);
            ++cnt[d];
            tmp = tmp / (seq_t)10;
        }
    }

    /* Build the inventory string in a buffer; the maximum length for any
       64‑bit number is below 30, so 256 bytes is more than enough for
       arbitrarily large k as well. */
    char buf[256];
    int len = 0;

    for (int d = 0; d <= 9; ++d) {
        unsigned c = cnt[d];
        if (c == 0) continue;

        /* Convert the count c to decimal digits in reverse order. */
        char rev[64];
        int revlen = 0;
        do {
            rev[revlen++] = '0' + (c % 10);
            c /= 10;
        } while (c > 0);

        /* Append the reversed digits to buf (now correct order). */
        for (int i = revlen - 1; i >= 0; --i) {
            buf[len++] = rev[i];
        }
        /* Append the digit itself. */
        buf[len++] = '0' + d;
    }

    /* Check palindrome. */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        if (buf[i] != buf[j]) return false;
    }
    return true;
}

/* Sequence entry function.  Uses a cache of previously found terms. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* dynamic array of terms */
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t next = (seq_t)1;      /* next integer to test */

    if (n == 0) return (seq_t)0;       /* safety, though sequence starts at 1 */

    /* Grow cache if necessary. */
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) { /* allocation failure – return 0 as error sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find terms up to index n. */
    while (cache_size < (size_t)n) {
        if (inventory_is_pal(next)) {
            cache[cache_size++] = next;
        }
        /* Increment next using addition to stay within allowed operators. */
        next = next + (seq_t)1;
    }
    return cache[n - 1];
}
