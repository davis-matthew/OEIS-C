/*
 * A037398  Numbers k such that every base-6 digit of k is a base-7 digit of
 *          k.
 * https://oeis.org/A037398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 5468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037398: Numbers k such that every base‑6 digit of k is a base‑7 digit of k.
   The function computes the n‑th term by a linear search and caches found
   terms for fast subsequent calls. */
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff every digit that appears in the base‑6 representation
   of k also appears in its base‑7 representation. */
static bool is_valid(seq_t k)
{
    unsigned mask6 = 0, mask7 = 0;
    seq_t t = k;

    /* base‑6 digits */
    while (t != 0) {
        unsigned d = (unsigned)(t % (seq_t)6);
        mask6 |= 1U << d;
        t /= (seq_t)6;
    }

    /* base‑7 digits */
    t = k;
    while (t != 0) {
        unsigned d = (unsigned)(t % (seq_t)7);
        mask7 |= 1U << d;
        t /= (seq_t)7;
    }

    /* every base‑6 digit must also be in the base‑7 digit set */
    return (mask6 & ~mask7) == 0;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache   = NULL;   /* array of seq_t values */
    static size_t cap     = 0;     /* allocated capacity */
    static size_t cnt     = 0;     /* number of terms cached */
    static seq_t next     = 1;     /* next candidate to test */

    /* Allocate initial capacity if needed */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until at least n are found */
    while (cnt < n) {
        /* Search for the next valid number */
        for (;;) {
            seq_t k = next;
            ++next;
            if (is_valid(k)) {
                /* Grow cache if necessary */
                if (cnt >= cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[cnt++] = k;
                break;
            }
        }
    }

    /* a(1) is the first element, so index n-1 */
    return cache[n - 1];
}
