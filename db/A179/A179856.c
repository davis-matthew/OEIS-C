/*
 * A179856  Numbers n such that n^3 can be obtained from n by inserting
 *          internal (but not necessarily contiguous) digits.
 * https://oeis.org/A179856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179856: numbers n such that n³ can be obtained from n by inserting
 * internal (but not necessarily contiguous) digits.
 *
 * The implementation tests successive integers for the following property:
 *   - cube(n) digits is a supersequence of n digits;
 *   - the first and last digits of n equal the first and last digits of cube(n);
 *   - at least one digit has been inserted (cube(n) has more digits than n).
 *
 * The sequence values are cached for efficiency.  For any order of calls this
 * function will return the correct value.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/get_digits.h"   /* Provides: void get_digits(seq_t, uint8_t *, size_t *); */

static bool is_valid_n(seq_t n)
{
    /* Compute cube */
    seq_t cube = n;
    cube *= cube;
    cube *= n;

    /* Obtain decimal digits of n and cube (most significant first) */
    uint8_t nd[64], cd[64];
    size_t nd_len, cd_len;

    get_digits(n, nd, &nd_len);
    get_digits(cube, cd, &cd_len);

    /* There must be at least one inserted digit */
    if (cd_len <= nd_len)
        return false;

    /* First and last digits must match */
    if (nd[0] != cd[0] || nd[nd_len - 1] != cd[cd_len - 1])
        return false;

    /* Verify inner digits of n appear in order within the inner digits of cube */
    size_t pos = 1;               /* position in cd, after the first digit */
    for (size_t i = 1; i + 1 < nd_len; ++i) {  /* iterate over inner digits of n */
        uint8_t d = nd[i];
        bool found = false;
        while (pos < cd_len - 1) {  /* stop before the last digit of cd */
            if (cd[pos] == d) {
                found = true;
                ++pos;
                break;
            }
            ++pos;
        }
        if (!found)
            return false;
    }
    return true;
}

/* Cache parameters – large enough to hold many terms for n up to ~10 000. */
#define CACHE_SIZE 200000
static seq_t cache[CACHE_SIZE];
static size_t cache_len = 0;
static seq_t next_candidate = (seq_t)1;

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Generate new terms until we have at least n terms in the cache. */
    while (cache_len < (size_t)n && cache_len < CACHE_SIZE) {
        if (is_valid_n(next_candidate)) {
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    /* Fallback: continue searching beyond the cache if needed. */
    seq_t val = next_candidate;
    while (!is_valid_n(val))
        ++val;
    return val;
}
