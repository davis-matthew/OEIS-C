/*
 * A039501  Iterations of "n->n/2 if n even, n->3n-1 if n odd" (A001281)
 *          starting at these numbers reach 5.
 * https://oeis.org/A039501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth integer that eventually reaches 5 under
   f(n) = n/2 if n even, f(n) = 3n-1 if n odd.
   Sequence a(n) is the list of such integers in increasing order,
   with a(1) = 5, a(2) = 7, ...
   All arithmetic is performed on seq_t values. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* Forward declaration of the public function */
seq_t a(idx_t n);

/* ----------------------------------------------------------- */
/* Test whether a given sequence number eventually reaches 5
   using Floyd's cycle‑finding algorithm.                       */
static bool reaches5(seq_t start) {
    seq_t tortoise = start;
    seq_t hare     = start;

    /* constants as seq_t */
    const seq_t ONE   = (seq_t)1;
    const seq_t TWO   = (seq_t)2;
    const seq_t THREE = (seq_t)3;
    const seq_t FIVE  = (seq_t)5;

    while (true) {
        /* advance tortoise one step */
        if (tortoise == FIVE) return true;
        if ((tortoise & ONE) != 0)
            tortoise = THREE * tortoise - ONE;   /* odd */
        else
            tortoise = tortoise >> 1;            /* even */

        /* advance hare two steps */
        if (hare == FIVE) return true;
        if ((hare & ONE) != 0)
            hare = THREE * hare - ONE;
        else
            hare = hare >> 1;

        if (hare == FIVE) return true;
        if ((hare & ONE) != 0)
            hare = THREE * hare - ONE;
        else
            hare = hare >> 1;

        /* if tortoise and hare meet, we are in a cycle not containing 5 */
        if (tortoise == hare) return false;
    }
}

/* ----------------------------------------------------------- */
/* Dynamic cache of the sequence values reached so far. */
static seq_t *cache    = NULL;
static size_t cache_cap = 0;   /* array capacity in seq_t elements */
static idx_t  cache_len  = 0;  /* number of entries stored */
static idx_t  next_candidate = 5;   /* next integer to test */

/* Ensure cache has capacity at least 'needed' */
static void ensure_cache_capacity(size_t needed) {
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap < needed) new_cap *= 2;
#ifdef __cplusplus
        seq_t *new_ptr = new seq_t[new_cap];
        if (cache) {
            std::copy(cache, cache + cache_len, new_ptr);
            delete[] cache;
        }
        cache = new_ptr;
#else
        seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* OOM: abort the program – this is a build failure */
            abort();
        }
        cache = new_ptr;
#endif
        cache_cap = new_cap;
    }
}

/* ----------------------------------------------------------- */
/* Public sequence function. */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* offset 1 – 0th element unused */

    /* grow cache until we have at least n entries */
    while (cache_len < n) {
        if (reaches5((seq_t)next_candidate)) {
            ensure_cache_capacity((size_t)cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
