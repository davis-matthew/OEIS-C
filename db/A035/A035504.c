/*
 * A035504  Numbers that eventually reach 1 under "x -> sum of cubes of digits
 *          of x".
 * https://oeis.org/A035504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th integer that eventually reaches 1 under the map
 *            x → sum of the cubes of its decimal digits. The function
 *            performs a brute‑force search with Floyd cycle detection
 * Range:     u8 n <= 2, u16 n <= 659, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Pre‑computed cubes of digits 0–9. */
static const seq_t cube[10] = {0,1,8,27,64,125,216,343,512,729};

/* Compute sum of cubes of decimal digits of v. */
static seq_t sum_cubes(seq_t v) {
    seq_t sum = 0;
    while (v) {
        unsigned int d = (unsigned int)(v % 10);
        sum += cube[d];
        v /= 10;
    }
    return sum;
}

/* Return non‑zero if v eventually reaches 1 under the digit‑cube map. */
static int reaches_one(seq_t v) {
    seq_t slow = v;
    seq_t fast = v;
    do {
        slow = sum_cubes(slow);
        fast = sum_cubes(sum_cubes(fast));
        if (slow == 1 || fast == 1)
            return 1;
        if (slow == fast)
            return 0;
    } while (1);
}

/* Cache of computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 1;      /* next integer to test */

/* Ensure the cache can hold at least n elements. */
static void ensure_cache_size(size_t n) {
    if (cache_len >= n) return;
    if (!cache_cap) cache_cap = 16;
    while (cache_cap < n) cache_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache = tmp;
}

/* Sequence generator. */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1 */
    ensure_cache_size((size_t)n);
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        if (reaches_one(val)) {
            cache[cache_len++] = val;
        }
        next_candidate++;
    }
    return cache[(size_t)n - 1];
}
