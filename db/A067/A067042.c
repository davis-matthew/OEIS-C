/*
 * A067042  Numbers in which the product of digits in even positions = product
 *          of digits in odd positions.
 * https://oeis.org/A067042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 3160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A067042: Numbers in which the product of digits in even positions
 * equals the product of digits in odd positions.
 * The algorithm tests consecutive integers, storing those that satisfy
 * the property.  It uses a cache so that multiple calls to a(n)
 * (in any order) are efficient.  Complexity is O(n · log a(n))
 * which is well within the 2 s limit for n up to 10 000.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if the decimal representation of x satisfies
 * the condition: product of digits in even positions = product of digits
 * in odd positions.  Numbers with fewer than two digits are not counted. */
static bool good(seq_t x)
{
    /* length of the decimal representation */
    size_t len = 0;
    /* temporary storage for digits (least‑significant first) */
    uint8_t *buf = NULL;
    size_t buf_cap = 0;
    seq_t t = x;

    if (t == 0) {          /* 0 has length 1, so it cannot be counted */
        return false;
    }

    /* Extract digits; grow buffer as needed */
    while (t > 0) {
        if (len == buf_cap) {
            size_t new_cap = (buf_cap == 0) ? 8 : buf_cap * 2;
            uint8_t *new_buf = (uint8_t *)realloc(buf, new_cap * sizeof(uint8_t));
            if (!new_buf) { free(buf); return false; }
            buf = new_buf;
            buf_cap = new_cap;
        }
        buf[len++] = (uint8_t)(t % 10);
        t /= 10;
    }

    if (len < 2) {
        free(buf);
        return false;           /* less than two digits → not counted */
    }

    seq_t prod_odd = 1, prod_even = 1;
    int pos = 1;               /* positions are counted from the left */

    for (int i = (int)len - 1; i >= 0; --i) {
        seq_t d = buf[i];
        if ((pos & 1) == 0) {
            prod_even *= d;
        } else {
            prod_odd  *= d;
        }
        ++pos;
    }

    free(buf);
    return prod_odd == prod_even;
}

/* ---------- cache for generated terms ---------- */
static seq_t *cache      = NULL;   /* array of terms          */
static size_t cache_size   = 0;    /* number of cached terms  */
static size_t cache_cap    = 0;    /* allocated capacity      */
static seq_t next_candidate = 1;   /* next integer to test   */

/* Ensure that the cache has at least 'cap' elements allocated. */
static void ensure_cache_cap(size_t cap)
{
    if (cache_cap < cap) {
        size_t new_cap = (cap > cache_cap) ? cap * 2 : cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);   /* allocation failure */
        cache     = new_cache;
        cache_cap = new_cap;
    }
}

/* Extend the cache up to (at least) index n (1‑based). */
static void extend_cache(size_t n)
{
    if (cache_size >= n) return;

    ensure_cache_cap(n);

    while (cache_size < n) {
        while (!good(next_candidate)) ++next_candidate;
        cache[cache_size++] = next_candidate;
        ++next_candidate;
    }
}

/* External function expected by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */
    extend_cache((size_t)n);
    return cache[n - 1];
}
