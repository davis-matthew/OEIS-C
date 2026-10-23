/*
 * A026455  a(n) = least positive integer < a(n-1) and not a(i)*a(j)-2 for
 *          1<=i<j<=n.
 * https://oeis.org/A026455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A026455 – “least positive integer > a(n‑1) and not a(i)*a(j)-2
 *  for 1≤i<j≤n”.  The function builds the sequence on demand,
 *  keeping a cache of computed terms and a bitset of all forbidden
 *  values a(i)*a(j)-2 for i<j up to the current index.
 *  It works for any seq_t/idx_t pair; for seq_t that is an arbitrary‑
 *  precision big integer the bitset indices are cast from seq_t and
 *  assume the values fit in size_t, which is true for n up to ≈10⁴.
 *  Initial value: a(1)=2. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* --------- Bitset of forbidden values ---------- */
static uint64_t *forb_bits   = NULL;
static size_t   forb_words   = 0;   /* number of allocated 64‑bit words */

/* Ensure bitset can address at least 'bit'.  Grow geometrically. */
static void
ensure_forb_bits(size_t bit)
{
    size_t needed_words = (bit + 64) >> 6;     /* ceil((bit+1)/64) */
    if (needed_words <= forb_words)
        return;

    size_t new_words = forb_words ? forb_words << 1 : 1;
    while (new_words < needed_words)
        new_words <<= 1;

    forb_bits = (uint64_t *)realloc(forb_bits, new_words * sizeof(uint64_t));
    size_t old_words = forb_words;
    /* zero the newly allocated words */
    memset(forb_bits + old_words, 0, (new_words - old_words) * sizeof(uint64_t));
    forb_words = new_words;
}

/* Mark a value as forbidden. */
static void
forbid_set(size_t bit)
{
    ensure_forb_bits(bit);
    forb_bits[bit >> 6] |= ((uint64_t)1 << (bit & 63));
}

/* Test whether a value is forbidden. */
static int
forbid_test(size_t bit)
{
    if (bit >> 6 >= forb_words)
        return 0;
    return ((forb_bits[bit >> 6] >> (bit & 63)) & 1) != 0;
}

/* --------- Cache of computed sequence terms ---------- */
static seq_t *cache     = NULL; /* 1‑based indexing: cache[1] = a(1) */
static size_t cache_cap = 0;    /* number of allocated slots (including 0th) */

/* Ensure the cache can store at least index n (1‑based). */
static void
ensure_cache_capacity(size_t n)
{
    if (cache_cap >= n + 1)   /* we keep an unused slot at 0 */
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < n + 1)
        new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* zero the newly allocated space for safety */
    if (new_cap > cache_cap)
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------------- Main sequence generator ---------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, return 0 */

    ensure_cache_capacity((size_t)n);

    /* Compute terms up to n if not already done */
    static idx_t cache_cnt = 0;   /* number of terms actually stored */

    while (cache_cnt < (size_t)n) {
        idx_t idx = ++cache_cnt;   /* new term index (1‑based) */

        seq_t next;
        if (idx == 1) {
            next = (seq_t)2;       /* a(1) = 2 */
            cache[idx] = next;
            continue;
        }

        /* Find the smallest integer > a(idx‑1) that is not forbidden. */
        seq_t cand = cache[idx - 1] + (seq_t)1;
        while (forbid_test((size_t)cand))
            cand += (seq_t)1;

        cache[idx] = cand;

        /* Mark forbidden values formed with this new term. */
        for (idx_t i = 1; i < idx; ++i) {
            seq_t prod = cache[i] * cand - (seq_t)2;
            forbid_set((size_t)prod);
        }
    }

    return cache[(size_t)n];
}
