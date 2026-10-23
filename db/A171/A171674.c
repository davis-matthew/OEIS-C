/*
 * A171674  Numbers n with property that n is divisible by (at least one) sum
 *          of two its positive digits.
 * https://oeis.org/A171674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171674: numbers divisible by the sum of two of their positive decimal digits.
   The sequence is generated on demand by testing successive integers for the property.
   Each term is stored once and reused on subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <new>
#endif

/* test whether a number has the required property */
static bool has_property(seq_t n)
{
    seq_t tmp = n;
    uint8_t digits[64];
    size_t dcount = 0;

    /* extract decimal digits */
    do {
        digits[dcount++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    if (dcount < 2) return false;          /* at least two digits required */

    /* count positive digits */
    size_t posCount = 0;
    for (size_t i = 0; i < dcount; ++i)
        if (digits[i] > 0) ++posCount;

    if (posCount < 2) return false;        /* at least two positive digits required */

    /* test all pairs of positive digits */
    for (size_t i = 0; i < dcount; ++i) {
        if (digits[i] == 0) continue;
        for (size_t j = i + 1; j < dcount; ++j) {
            if (digits[j] == 0) continue;
            seq_t sum = (seq_t)digits[i] + (seq_t)digits[j];
            if (sum == 0) continue;       /* avoid division by zero */
            if (n % sum == 0) return true;
        }
    }
    return false;
}

#ifdef __cplusplus
/* C++: cache is an array of pointers to seq_t objects */
static seq_t **cache_ptr = nullptr;
static idx_t cache_len = 0;
static size_t cache_cap = 0;
#else
/* C: cache is an array of seq_t values */
static seq_t *cache_arr = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;
#endif

static seq_t next_candidate = 1;

/* Ensure the cache can hold at least 'min_cap' terms */
static void ensure_cache_capacity(size_t min_cap)
{
#ifdef __cplusplus
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t **tmp = (seq_t**)realloc(cache_ptr, new_cap * sizeof(seq_t*));
    if (!tmp) abort();
    cache_ptr = tmp;
    cache_cap = new_cap;
#else
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t*)realloc(cache_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache_arr = tmp;
    cache_cap = new_cap;
#endif
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 */

    while (cache_len < n) {
        while (!has_property(next_candidate))
            ++next_candidate;

        ensure_cache_capacity(cache_len + 1);
#ifdef __cplusplus
        cache_ptr[cache_len++] = new seq_t(next_candidate);
#else
        cache_arr[cache_len++] = next_candidate;
#endif
        ++next_candidate;
    }

#ifdef __cplusplus
    return *(cache_ptr[n - 1]);
#else
    return cache_arr[n - 1];
#endif
}
