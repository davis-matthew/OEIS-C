/*
 * A037986  Numbers whose maximal base 6 run length is 3.
 * https://oeis.org/A037986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037986: numbers whose maximal base-6 run length is exactly 3.
   Formula: a(n) = the n-th positive integer whose base‑6 representation
   has maximal run length exactly 3. */

/*-----------------------------------------------------------*/
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif
/*-----------------------------------------------------------*/

/* Cache for generated terms ------------------------------------*/
#ifdef __cplusplus
/* C++ build – use a std::vector for automatic construction */
static std::vector<seq_t> cache_vec;
#else
/* C build – raw array with manual reallocation */
static seq_t *seq_cache = NULL;
static idx_t  seq_cache_len = 0;
static idx_t  seq_cache_cap = 0;
#endif

/* Current length of base‑6 numbers to generate (starts at 3) */
static idx_t next_length = 3;

/* Utility: push a new term into the cache ----------------------*/
static void push_cache(seq_t val) {
#ifdef __cplusplus
    cache_vec.push_back(val);
#else
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_len++] = val;
#endif
}

static idx_t cache_size(void) {
#ifdef __cplusplus
    return cache_vec.size();
#else
    return seq_cache_len;
#endif
}

static seq_t cache_get(idx_t idx) {
#ifdef __cplusplus
    return cache_vec[idx];
#else
    return seq_cache[idx];
#endif
}

/* Recursive generator for a fixed length L ---------------------*/
static void gen_rec(idx_t pos, idx_t L, seq_t cur_val,
                    int last_digit, int run_len, bool has_run3) {
    if (pos == L) {
        if (has_run3)                 /* valid number – store it */
            push_cache(cur_val);
        return;
    }
    for (int d = 0; d < 6; ++d) {
        int new_run = (d == last_digit) ? run_len + 1 : 1;
        if (new_run > 3)              /* would create a run of 4 */
            continue;
        bool new_has = has_run3 || (new_run == 3);
        seq_t new_val = cur_val * (seq_t)6 + d;
        gen_rec(pos + 1, L, new_val, d, new_run, new_has);
    }
}

/* Generate all valid numbers of a specific length L -----------*/
static void generate_length(idx_t L) {
    /* Leading digit cannot be zero. */
    for (int first = 1; first <= 5; ++first) {
        gen_rec(1, L, (seq_t)first, first, 1, false);
    }
}

/* Sequence function -------------------------------------------*/
seq_t a(idx_t n) {
    /* Ensure we have at least n terms in the cache. */
    while (cache_size() < n) {
        generate_length(next_length);
        ++next_length;
    }
    return cache_get(n - 1);
}
