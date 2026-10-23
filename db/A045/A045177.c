/*
 * A045177  Numbers whose base-5 representation contains exactly one 0 and one
 *          2.
 * https://oeis.org/A045177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains exactly one 0 and
 *            one 2
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically grown array of terms   */
static size_t cache_size   = 0;      /* number of terms currently cached   */
static size_t cache_cap    = 0;      /* current capacity of the array      */
static idx_t   next_len_gen = 2;     /* next base‑5 length to generate     */

/* Grow the cache to at least new_cap elements.  Uses C++ new/delete
   when compiled as C++ to correctly construct objects. */
static void grow_cache(size_t new_cap) {
#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        delete [] cache;
    }
    cache = tmp;
    cache_cap = new_cap;
#else
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
#endif
}

/* Append a new term to the cache, growing capacity if needed. */
static void add_value(seq_t val) {
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        grow_cache(new_cap);
    }
    cache[cache_size++] = val;
}

/* Recursive generation of all base‑5 numbers of a given length that
   contain exactly one digit 0 and one digit 2. */
static void gen_len_rec(idx_t len, idx_t pos,
                        int zeros_left, int twos_left, seq_t val) {
    if (pos == len) {                 /* all digits processed */
        if (zeros_left == 0 && twos_left == 0)
            add_value(val);
        return;
    }
    int leading = (pos == 0);
    for (int d = 0; d < 5; ++d) {
        if (leading && d == 0) continue;               /* no leading zero */
        if (d == 0 && zeros_left == 0) continue;       /* too many zeros */
        if (d == 2 && twos_left == 0) continue;        /* too many twos */
        int nz = zeros_left, nt = twos_left;
        if (d == 0) --nz;
        if (d == 2) --nt;
        seq_t new_val = val * (seq_t)5 + (seq_t)d;
        gen_len_rec(len, pos + 1, nz, nt, new_val);
    }
}

/* Generate all numbers of base‑5 length `len`. */
static void generate_length(idx_t len) {
    gen_len_rec(len, 0, 1, 1, (seq_t)0);
}

/* Public function: returns the n‑th term (1‑based). */
seq_t a(idx_t n) {
    if (!cache) {
        /* initial allocation */
#if defined(__cplusplus)
        size_t init_cap = 16;
        seq_t *tmp = new seq_t[init_cap];
        cache = tmp;
        cache_cap = init_cap;
#else
        size_t init_cap = 16;
        seq_t *tmp = malloc(init_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = init_cap;
#endif
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        generate_length(next_len_gen);
        ++next_len_gen;
    }
    return cache[n - 1];
}
