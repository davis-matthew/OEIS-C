/*
 * A044996  Numbers whose base-3 representation contains exactly one 0 and two
 *          2's.
 * https://oeis.org/A044996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 870, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044996: Numbers whose base‑3 representation contains exactly one 0 and two 2's.
   The implementation enumerates all valid base‑3 numbers, caches them, and
   returns the n‑th element. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Dynamic cache for generated terms. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Temporary table of powers of 3: pow3[k] = 3^k. */
static seq_t pow3[128];
static idx_t pow3_size = 0;

/* Append a value to the cache, reallocating if necessary. */
static void append_value(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Ensure the pow3 array contains all powers up to max_len. */
static void ensure_pow3(idx_t max_len)
{
    if (pow3_size >= max_len) return;
    if (pow3_size == 0) { pow3[0] = (seq_t)1; pow3_size = 1; }
    for (idx_t i = pow3_size; i <= max_len; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    pow3_size = max_len + 1;
}

/* Recursive enumeration of length‑L base‑3 numbers with the required digit
   pattern. */
static void gen_rec(idx_t pos, bool zero_used, idx_t two_used,
                    seq_t value, idx_t L)
{
    if (pos == L) {
        if (zero_used && two_used == 2) {
            append_value(value);
        }
        return;
    }

    idx_t remain = L - pos - 1;   /* positions yet to fill */

    if (pos == 0) {
        /* Most significant digit cannot be 0. */
        for (int d = 1; d <= 2; ++d) {
            bool nz = zero_used || (d == 0);
            idx_t ntw = two_used + (d == 2);
            if (ntw > 2) continue;
            idx_t need_zero = nz ? 0 : 1;
            idx_t need_two  = 2 - ntw;
            if (need_zero > remain || need_two > remain) continue;

            seq_t add = (seq_t)d * pow3[remain];
            gen_rec(pos + 1, nz, ntw, value + add, L);
        }
    } else {
        for (int d = 0; d <= 2; ++d) {
            /* Disallow a second zero. */
            if (zero_used && d == 0) continue;

            bool nz = zero_used || (d == 0);
            idx_t ntw = two_used + (d == 2);
            if (ntw > 2) continue;
            idx_t need_zero = nz ? 0 : 1;
            idx_t need_two  = 2 - ntw;
            if (need_zero > remain || need_two > remain) continue;

            seq_t add = (seq_t)d * pow3[remain];
            gen_rec(pos + 1, nz, ntw, value + add, L);
        }
    }
}

/* Generate terms until we have at least target_n entries. */
static void build_up_to(idx_t target_n)
{
    if (cache_len >= target_n) return;

    static idx_t current_len = 2;   /* last processed length */
    while (cache_len < target_n) {
        ++current_len;
        ensure_pow3(current_len);
        gen_rec(0, false, 0, (seq_t)0, current_len);
    }
}

/* Public function: return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, return 0 */
    build_up_to(n);
    return cache[n - 1];
}
