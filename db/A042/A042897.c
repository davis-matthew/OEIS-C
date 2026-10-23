/*
 * A042897  Denominators of continued fraction convergents to sqrt(980).
 * https://oeis.org/A042897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042897: Denominators of continued‑fraction convergents to sqrt(980)
   Recurrence: a(n) = 103682 * a(n-10) – a(n-20)   for n > 19
   Base values:
   a(0)=1, a(1)=3, a(2)=10, a(3)=13, a(4)=23, a(5)=59, a(6)=82, a(7)=141,
   a(8)=505, a(9)=1656, a(10)=103177, a(11)=311187, a(12)=1036738,
   a(13)=1347925, a(14)=2384663, a(15)=6117251, a(16)=8501914,
   a(17)=14619165, a(18)=52359409, a(19)=171697392 */

#include <stddef.h>
#include <stdlib.h>

/* ---------- Base values ---------- */
static const seq_t init_20[20] = {
    (seq_t)1,   (seq_t)3,   (seq_t)10,  (seq_t)13,  (seq_t)23,
    (seq_t)59,  (seq_t)82,  (seq_t)141, (seq_t)505, (seq_t)1656,
    (seq_t)103177, (seq_t)311187, (seq_t)1036738, (seq_t)1347925,
    (seq_t)2384663, (seq_t)6117251, (seq_t)8501914, (seq_t)14619165,
    (seq_t)52359409, (seq_t)171697392
};

/* ---------- Dynamic cache ---------- */
static seq_t *cache   = NULL;   /* array of computed terms */
static size_t cache_len = 0;    /* number of terms currently stored (>=20 after init) */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure the cache holds indices up to n (inclusive) and
   initialise the first 20 terms if not already done. */
static void ensure_cache_size(size_t n)
{
    /* Grow capacity if necessary */
    if (cache_cap <= n) {
        size_t new_cap = (n < 20) ? 20 : n + 1;   /* plus one because index starts at 0 */
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_mem;
        cache_cap = new_cap;
    }

    /* Initialise base terms once */
    if (cache_len < 20) {
        for (size_t i = cache_len; i < 20; ++i)
            cache[i] = init_20[i];
        cache_len = 20;
    }
}

/* ---------- Sequence generator ---------- */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Direct lookup for the first 20 terms */
    if (idx < 20)
        return init_20[idx];

    /* Make sure the cache can hold the desired index */
    ensure_cache_size(idx);

    /* Compute any missing terms on demand */
    if (cache_len <= idx) {
        for (size_t k = cache_len; k <= idx; ++k) {
            cache[k] = (seq_t)103682 * cache[k - 10] - cache[k - 20];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
