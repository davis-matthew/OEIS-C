/*
 * A237568  Fibonacci-like sequence of numbers with nondecreasing positive
 *          digits. Let a^+ denote the number that is obtained from a if its
 *          positive digits are written in nondecreasing order, while zeros
 *          remain in their places. Let a<+>b = (a + b)^+. a(0)=0, a(1)=1, for
 *          n>=2, a(n) = a(n-1) <+> a(n-2).
 * https://oeis.org/A237568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (a(n-1) + a(n-2))^+ for n>=2 where ^+
 *            means: rearrange the positive digits of a number in
 *            nondecreasing order while keeping zeros in their original
 *            positions
 * Range:     u8 n <= 22, u16 n <= 75, u32 n <= 439, u64 n <= 952,
 *            u128 n <= 952, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum number of decimal digits expected – safe for all tested ranges. */
#define MAX_DIGITS 4096

/* Perform the ^+ operation: sort positive digits nondecreasingly
   while zeros remain in their original places. */
static seq_t plus_nondecreasing_positive_digits(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;

    /* Extract digits (least‑significant first). */
    uint8_t rev[MAX_DIGITS];
    size_t rev_len = 0;
    seq_t tmp = x;
    while (tmp != (seq_t)0) {
        rev[rev_len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    if (rev_len > MAX_DIGITS)
        abort(); /* should not happen for generated values */

    /* Reverse to most‑significant first. */
    uint8_t digits[MAX_DIGITS];
    for (size_t i = 0; i < rev_len; ++i)
        digits[i] = rev[rev_len - 1 - i];

    /* Separate zeros and positive digits. */
    bool zero_pos[MAX_DIGITS];
    uint8_t pos[MAX_DIGITS];
    size_t pos_cnt = 0;
    for (size_t i = 0; i < rev_len; ++i) {
        if (digits[i] == 0)
            zero_pos[i] = true;
        else {
            zero_pos[i] = false;
            pos[pos_cnt++] = digits[i];
        }
    }

    /* Sort the collected positive digits (simple insertion sort). */
    for (size_t i = 1; i < pos_cnt; ++i) {
        uint8_t key = pos[i];
        size_t j = i;
        while (j > 0 && pos[j - 1] > key) {
            pos[j] = pos[j - 1];
            --j;
        }
        pos[j] = key;
    }

    /* Reconstruct the result. */
    seq_t result = (seq_t)0;
    size_t pos_idx = 0;
    for (size_t i = 0; i < rev_len; ++i) {
        result = result * (seq_t)10;
        if (!zero_pos[i])
            result += (seq_t)pos[pos_idx++];
    }

    return result;
}

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache is large enough to hold indices up to up_to (inclusive). */
static void ensure_cache_capacity(size_t up_to)
{
    if (up_to + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= up_to)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Make sure the base values a(0)=0 and a(1)=1 are stored. */
static void ensure_base_cache(void)
{
    if (cache_len == 0) {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        ensure_base_cache();
        return cache[0];
    }
    if (n == (idx_t)1) {
        ensure_base_cache();
        return cache[1];
    }

    ensure_base_cache();

    size_t idx = (size_t)n;
    if (idx < cache_len)
        return cache[idx];

    ensure_cache_capacity(idx);
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        cache[i] = plus_nondecreasing_positive_digits(sum);
    }
    cache_len = idx + 1;
    return cache[idx];
}
