/*
 * A044888  Numbers whose base-2 run lengths alternate: even, odd, even, ...
 * https://oeis.org/A044888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose binary run lengths alternate even
 *            (ones), odd (zeros), even (ones), … (OEIS A044888)
 * Range:     u8 n <= 20, u16 n <= 520, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compare_seq_t.h" /* compare_seq_t */

/* Working cache – large enough to hold a few tens of thousands of terms.
   For primitive unsigned types this is a plain array; for arbitrary‑precision
   types the compiler will generate a static array of that type. */
#define CACHE_MAX 100000
static seq_t cache[CACHE_MAX];
static size_t cache_cnt = 0;
static idx_t   cache_bits = 0;   /* largest bit length that has been generated */

/* Generate recursively all numbers that satisfy the alternating‐run rule
   up to a maximum bit length.  Generated values are appended to `arr`
   and `cnt` is incremented. */
static void add_runs(seq_t val, idx_t run, idx_t cur_len,
                     idx_t max_bits, seq_t *arr, size_t *cnt)
{
    /* Run 0 is a run of 1s, run 1 a run of 0s, run 2 again 1s, …   */
    bool ones = (run & 1) == 0;
    idx_t min_len = ones ? 2 : 1; /* even ones, odd zeros */

    for (idx_t len = min_len; cur_len + len <= max_bits; len += 2)
    {
        seq_t new_val = val << len;
        if (ones)                     /* append len ones */
        {
            seq_t mask = ((seq_t)1 << len) - 1;
            new_val |= mask;
        }
        if (*cnt < CACHE_MAX)
            arr[(*cnt)++] = new_val;
        add_runs(new_val, run + 1, cur_len + len, max_bits, arr, cnt);
    }
}

/* Build cache up to a given bit length.  Existing cache is discarded. */
static void build_cache(idx_t bits)
{
    seq_t temp[CACHE_MAX];
    size_t cnt = 0;

    /* Start with a run of 1s with even length. */
    for (idx_t l0 = 2; l0 <= bits; l0 += 2)
    {
        seq_t first = ((seq_t)1 << l0) - 1;   /* l0 ones */
        if (cnt < CACHE_MAX)
            temp[cnt++] = first;
        add_runs(first, 1, l0, bits, temp, &cnt);
    }

    /* Sort to obtain terms in increasing order. */
    qsort(temp, cnt, sizeof(seq_t), compare_seq_t);

    /* Copy to global cache. */
    for (size_t i = 0; i < cnt && i < CACHE_MAX; ++i)
        cache[i] = temp[i];
    cache_cnt  = cnt;
    cache_bits = bits;
}

/* Public sequence function.  The sequence is memoised; calls may come
   in any order.  The cache grows until it contains at least n terms. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety for 0‑index */

    /* Grow cache until we have at least n terms. */
    while (cache_cnt < (size_t)n)
    {
        /* Increase bit length in steps of two (maintains alternating rule). */
        idx_t next_bits = (cache_bits == 0) ? 2 : cache_bits + 2;

        /* For fixed 64‑bit types we cannot shift beyond 63 bits safely. */
        if (sizeof(seq_t) <= sizeof(unsigned long long))
            if (next_bits > 63)
                break;

        build_cache(next_bits);
    }

    /* Cache is guaranteed to contain at least n terms now. */
    return cache[(size_t)(n - 1)];
}
