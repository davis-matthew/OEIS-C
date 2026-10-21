/*
 * A065983  Palindromes whose digit sum is 4.
 * https://oeis.org/A065983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate all decimal palindromes whose digit sum is 4, in ascending
   numerical order.  */
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* seq_t and idx_t are supplied by the build system. */
/* The following static variables hold the memoised sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static size_t last_processed_length = 0;

/* Ensure the cache has capacity 'needed', allocating or re‑allocating if
   necessary.  The implementation differs for C and C++ builds. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;

#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_size; ++i) newp[i] = cache[i];
    delete[] cache;
    cache = newp;
#else
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();
    cache = newp;
#endif
    cache_cap = new_cap;
}

/* Append a palindrome value to the cache. */
static void push_val(seq_t v)
{
    ensure_capacity(cache_size + 1);
    cache[cache_size++] = v;
}

/* Recursively build even‑length palindromes.  'left' holds the left half
   of the palindrome; 'pos' is the current position being set; 'rem'
   is the remaining sum needed for the left half; 'm' is the length of
   the left half. */
static void rec_even_len(uint8_t *left, size_t pos, int rem, size_t m)
{
    if (pos == m) {
        if (rem == 0) {
            seq_t val = 0;
            for (size_t i = 0; i < m; ++i) val = val * 10 + left[i];
            for (size_t i = m; i > 0; --i) val = val * 10 + left[i - 1];
            push_val(val);
        }
        return;
    }

    int low = (pos == 0) ? 1 : 0;
    int high = rem;
    if (high > 9) high = 9;

    for (int d = low; d <= high; ++d) {
        left[pos] = (uint8_t)d;
        rec_even_len(left, pos + 1, rem - d, m);
    }
}

/* Recursively build odd‑length palindromes.  'middle' is the centre digit. */
static void rec_odd_len(uint8_t *left, size_t pos, int rem,
                       size_t m, int middle)
{
    if (pos == m) {
        if (rem == 0) {
            seq_t val = 0;
            for (size_t i = 0; i < m; ++i) val = val * 10 + left[i];
            val = val * 10 + middle;
            for (size_t i = m; i > 0; --i) val = val * 10 + left[i - 1];
            push_val(val);
        }
        return;
    }

    int low = (pos == 0) ? 1 : 0;
    int high = rem;
    if (high > 9) high = 9;

    for (int d = low; d <= high; ++d) {
        left[pos] = (uint8_t)d;
        rec_odd_len(left, pos + 1, rem - d, m, middle);
    }
}

/* Generate all palindromes of a single decimal length L and append them
   to the cache.  L = 1 is handled as a special case. */
static void generate_length(size_t L)
{
    if (L == 1) {
        push_val((seq_t)4);
        return;
    }

    size_t left_len = (L + 1) / 2;
    uint8_t *left = (uint8_t *)malloc(left_len * sizeof(uint8_t));
    if (!left) abort();

    if ((L & 1) == 0) {          // even length
        size_t m = L / 2;
        rec_even_len(left, 0, 2, m);
    } else {                     // odd length
        size_t m = (L - 1) / 2;

        /* middle digit 2, left sum 1 */
        rec_odd_len(left, 0, 1, m, 2);

        /* middle digit 0, left sum 2 */
        rec_odd_len(left, 0, 2, m, 0);
    }

    free(left);
}

/* Public sequence function.  Returns the n‑th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Generate palindromes until we have at least n terms cached. */
    while (cache_size < (size_t)n) {
        ++last_processed_length;
        generate_length(last_processed_length);
    }
    return cache[(size_t)n - 1];
}
