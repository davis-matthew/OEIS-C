/*
 * A045235  Numbers whose base-5 representation contains no 1's and exactly
 *          three 3's.
 * https://oeis.org/A045235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth number whose base‑5 representation * contains no
 *            digit 1 and exactly three digits 3. The sequence * is generated
 *            incrementally; no closed‑form formula is known
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* cached sequence values            */
static size_t cache_cap = 0;         /* current capacity of cache          */
static size_t cache_cnt = 0;         /* number of terms currently cached   */
static idx_t  max_len_done = 0;      /* longest length processed so far    */

/* Ensure the cache can hold at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 256;
    if (new_cap < needed) new_cap = needed;

    if (!cache) {
        cache = seq_alloc(new_cap);
        if (!cache) abort();
    } else {
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();
        cache = tmp;
    }
    cache_cap = new_cap;
}

/* Recursive construction of all d‑digit numbers that
 * (1) contain exactly three 3's,
 * (2) contain no digit 1,
 * (3) have no leading zero.
 * Digits are tried in ascending order to preserve numeric order. */
static void generate_len_rec(idx_t d, idx_t pos, int rem3, seq_t val)
{
    if (pos == d) {                    /* a full number has been formed */
        if (rem3 == 0) {
            ensure_capacity(cache_cnt + 1);
            cache[cache_cnt++] = val;
        }
        return;
    }

    static const uint8_t digs[4] = {0, 2, 3, 4};
    for (int i = 0; i < 4; ++i) {
        uint8_t dgt = digs[i];
        if (pos == 0 && dgt == 0) continue;          /* no leading zero */

        int new_rem3 = rem3 - (dgt == 3);
        if (new_rem3 < 0) continue;

        idx_t rest = d - pos - 1;
        if ((int)rest < new_rem3) continue;           /* impossible to fill */

        seq_t next_val = val * (seq_t)5 + (seq_t)dgt;
        generate_len_rec(d, pos + 1, new_rem3, next_val);
    }
}

/* Generate all numbers of length `d` and append them to the cache. */
static void generate_length(idx_t d)
{
    if (d < 3) return;                /* at least three 3's are required */
    generate_len_rec(d, 0, 3, (seq_t)0);
}

/* Public interface: return the nth term (a(1) = 93 for all pre‑alocated
 * seq_t types that can store the value). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n = 0 */

    while (cache_cnt < n) {
        ++max_len_done;
        generate_length(max_len_done);
    }
    return cache[n - 1];
}
