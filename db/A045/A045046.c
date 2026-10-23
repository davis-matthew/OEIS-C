/*
 * A045046  Numbers whose base-4 representation contains exactly one 0 and
 *          four 2's.
 * https://oeis.org/A045046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base-4 representation contains exactly one
 *            0 and four 2's
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Forward declarations of internal helpers */
static void add_to_cache(seq_t val);
static void gen_len_rec(size_t L, size_t pos, seq_t val, int rem_zero, int rem_twos);
static void generate_until(idx_t target);

/* Static memory for sequence cache */
static seq_t  *cache = NULL;
static idx_t   cache_len = 0;
static size_t  cache_cap = 0;
static size_t  cur_len = 5;   /* next length to generate */

/* Append a new value to the cache, growing storage as needed */
static void add_to_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            abort();  /* allocation failure */
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive helper that builds all numbers of a given length L that
   satisfy the digit constraints. 'pos' counts from 1 (next position
   after the already fixed leading digit)' */
static void gen_len_rec(size_t L, size_t pos, seq_t val, int rem_zero, int rem_twos) {
    if (pos == L) {
        if (rem_zero == 0 && rem_twos == 0) {
            add_to_cache(val);
        }
        return;
    }
    size_t remaining = L - pos; /* positions left INCLUDING current? */
    for (int d = 0; d <= 3; ++d) {
        int rz = rem_zero, rt = rem_twos;
        if (d == 0) {
            if (rz == 0) continue;
            rz--;
        } else if (d == 2) {
            if (rt == 0) continue;
            rt--;
        }
        size_t still_need = remaining - 1;
        if (rz + rt > (int)still_need) continue;
        if (rt > (int)still_need) continue;
        if (rz > (int)still_need) continue;
        gen_len_rec(L, pos + 1, val * 4 + d, rz, rt);
    }
}

/* Generate sequence entries up to at least 'target' terms */
static void generate_until(idx_t target) {
    while (cache_len < target) {
        size_t L = cur_len; /* current length to generate */
        for (int lead = 1; lead <= 3; ++lead) {
            seq_t  val = (seq_t)lead;
            int rem_zero = 1;                    /* always need exactly one 0 */
            int rem_twos = 4 - (lead == 2 ? 1 : 0); /* consume one 2 if leading is 2 */
            if (rem_zero > (int)(L - 1) || rem_twos > (int)(L - 1)) {
                continue; /* impossible with this leading digit */
            }
            gen_len_rec(L, 1, val, rem_zero, rem_twos);
        }
        ++cur_len; /* next length to process */
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0; /* undefined for index 0; return 0 */
    }
    if (n > cache_len) {
        generate_until(n);
    }
    return cache[n - 1];
}
