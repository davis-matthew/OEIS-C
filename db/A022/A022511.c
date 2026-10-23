/*
 * A022511  Describe previous term from the right (method A - initial term is
 *          7).
 * https://oeis.org/A022511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0) = 7. For n > 0, a(n) is obtained by describing the decimal
 *            representation of a(n‑1) from the right: repeatedly group
 *            identical right‑most digits, count the size of each group, and
 *            output the count followed by the digit. For example, a(1) = 17
 *            (one 7), a(2) = 1711 (two 1's, one 7, one 1), a(3...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Forward declaration of the function that computes the next term */
static seq_t compute_next(seq_t prev);

/* #################### C++ implementation #################### */
#if defined(__cplusplus)

/* Use a std::vector to store the terms in C++ builds */
static std::vector<seq_t> cache_vec;

/* Compute the next term from the previous one */
static seq_t compute_next(seq_t prev)
{
    seq_t result = 0;
    seq_t cur = prev;
    while (cur > 0) {
        seq_t digit = cur % 10;

        /* Count how many of the current rightmost digit are equal */
        seq_t tmp = cur;
        size_t count = 0;
        while (tmp > 0 && (tmp % 10) == digit) {
            ++count;
            tmp /= 10;
        }
        cur = tmp;

        /* Append the decimal representation of count (in normal order) */
        uint8_t buf[32];
        size_t pos = 0;
        size_t c = count;
        if (c == 0)
            buf[pos++] = 0;
        else
            while (c > 0) {
                buf[pos++] = c % 10;
                c /= 10;
            }
        for (size_t i = pos; i > 0; --i)
            result = result * 10 + buf[i - 1];

        /* Append the digit itself */
        result = result * 10 + (seq_t)digit;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Initialise the cache on first use */
    if (cache_vec.empty()) {
        cache_vec.reserve(2);
        cache_vec.push_back((seq_t)7);
    }

    /* Generate terms up to n */
    while (cache_vec.size() <= n) {
        seq_t next = compute_next(cache_vec.back());
        cache_vec.push_back(next);
    }
    return cache_vec[(size_t)n];
}

/* #################### C implementation #################### */
#else

/* In C builds we use a manually allocated dynamic array */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Compute the next term from the previous one (same algorithm as above) */
static seq_t compute_next(seq_t prev)
{
    seq_t result = 0;
    seq_t cur = prev;
    while (cur > 0) {
        seq_t digit = cur % 10;

        seq_t tmp = cur;
        size_t count = 0;
        while (tmp > 0 && (tmp % 10) == digit) {
            ++count;
            tmp /= 10;
        }
        cur = tmp;

        uint8_t buf[32];
        size_t pos = 0;
        size_t c = count;
        if (c == 0)
            buf[pos++] = 0;
        else
            while (c > 0) {
                buf[pos++] = c % 10;
                c /= 10;
            }
        for (size_t i = pos; i > 0; --i)
            result = result * 10 + buf[i - 1];

        result = result * 10 + (seq_t)digit;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Initialise the cache */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    if (cache_len == 0) {
        cache[0] = (seq_t)7;
        cache_len = 1;
    }

    /* Grow the cache until it contains n+1 terms */
    while (cache_len <= (size_t)n) {
        seq_t next = compute_next(cache[cache_len - 1]);

        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = next;
    }
    return cache[(size_t)n];
}

#endif
