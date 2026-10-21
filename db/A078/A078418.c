/*
 * A078418  Numbers k such that h(k) = h(k-1) + h(k-2), where h(k) =
 *          A006577(k) + 1 is the length of the sequence {k, f(k), f(f(k)),
 *          ...., 1} in the Collatz (or 3x + 1) problem. (The earliest "1" is
 *          meant.)
 * https://oeis.org/A078418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that h(k) = h(k‑1) + h(k‑2), *
 *            where h(k) = A006577(k) + 1 is the length of the Collatz
 *            sequence * starting at k. The function builds the list lazily
 *            using a cached * computation of h(k). No closed form is known,
 *            so a search with memoisation * is performed. The rout...
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*--- static storage for cached h(k) values ---------------------------------*/
static seq_t *h_cache = NULL;      /* index 0 unused */
static size_t cache_size = 0;      /* current allocated size in seq_t’s */
static bool h_initialized = false; /* lazy initialisation flag */

/*--- temporary stack for a Collatz path -----------------------------------*/
static idx_t *path_buf = NULL;
static size_t path_cap = 0;

/*--- list of numbers satisfying the condition -----------------------------*/
static seq_t *seq_list = NULL;
static size_t list_len = 0;
static size_t list_cap = 0;
static idx_t max_checked = 0;

/*--- ensure h_cache has room for indices up to up_to ---------------------*/
static void ensure_cache(idx_t up_to) {
    if ((size_t)up_to < cache_size) return;
    size_t new_size = cache_size > 0 ? cache_size : 1024;
    while (new_size <= (size_t)up_to) new_size <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(h_cache, new_size * sizeof(seq_t));
    if (!new_ptr) abort();
    for (size_t i = cache_size; i < new_size; ++i) new_ptr[i] = 0;
    h_cache = new_ptr;
    cache_size = new_size;
}

/*--- initialise h_cache for h(1) -----------------------------------------*/
static void init_h_cache(void) {
    if (!h_initialized) {
        ensure_cache(1);
        h_cache[1] = 1;            /* h(1) = 1 */
        h_initialized = true;
    }
}

/*--- compute h(n) with memoisation --------------------------------------*/
static seq_t get_len(idx_t n) {
    init_h_cache();
    ensure_cache(n);
    if (h_cache[n] != 0) return h_cache[n];

    idx_t cur = n;
    size_t path_len = 0;

    /* walk the Collatz path until a cached value is reached */
    while (true) {
        if (cur < (idx_t)cache_size && h_cache[cur] != 0) break;

        if (path_len == path_cap) {
            size_t new_cap = path_cap ? path_cap << 1 : 64;
            idx_t *new_buf = (idx_t *)realloc(path_buf, new_cap * sizeof(idx_t));
            if (!new_buf) abort();
            path_buf = new_buf;
            path_cap = new_cap;
        }
        path_buf[path_len++] = cur;

        cur = (cur & 1) ? 3 * cur + 1 : cur >> 1;
    }

    seq_t val = h_cache[cur];   /* base value */

    /* back‑track, filling cache where possible */
    for (size_t i = path_len; i-- > 0; ) {
        ++val;
        if (path_buf[i] < (idx_t)cache_size)
            h_cache[path_buf[i]] = val;
    }

    return h_cache[n];
}

/*--- append a new element to seq_list -----------------------------------*/
static void append_seq(seq_t v) {
    if (list_len == list_cap) {
        size_t new_cap = list_cap ? list_cap << 1 : 4;
        seq_t *new_arr = (seq_t *)realloc(seq_list, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        seq_list = new_arr;
        list_cap = new_cap;
    }
    seq_list[list_len++] = v;
}

/*--- public function -----------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) abort();              /* sequence is 1‑based */
    while (list_len < (size_t)n) {
        ++max_checked;
        seq_t hk = get_len(max_checked);
        if (max_checked >= 3) {
            seq_t hkm1 = h_cache[max_checked - 1];
            seq_t hkm2 = h_cache[max_checked - 2];
            if (hk == hkm1 + hkm2)
                append_seq((seq_t)max_checked);
        }
    }
    return seq_list[n - 1];
}
