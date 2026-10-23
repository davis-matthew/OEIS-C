/*
 * A163759  Exactly three distinct primes occur as substrings of the digits of
 *          n.
 * https://oeis.org/A163759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth integer (n ≥ 1) that has exactly three distinct
 *            prime * substrings in its decimal representation. * * The
 *            function builds the sequence incrementally, caching results
 *            between * calls. It only relies on the utility functions
 *            supplied in the build * environment
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_len, free_seq_array, is_prime_big, seq_alloc_new_array,
 *            seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"
#include "../utils/seq_from_str.h"
#include "../utils/free_seq_array.h"
#include "../utils/seq_alloc_new_array.h"

typedef uint64_t idx_t; /* for compatibility with the build */

/* ---------- helper to manage the cached sequence ---------- */

static seq_t *cached_values = NULL;   /* pointer to array of values         */
static size_t cached_len   = 0;        /* number of values stored            */
static size_t cached_cap   = 0;        /* current allocated capacity         */

/* current number being tested for inclusion */
static seq_t cur_num = 0;
static bool cur_initialized = false;

/* expand cached array to at least 'needed' capacity */
static void expand_cache(size_t needed)
{
    if (needed <= cached_cap)
        return;
    size_t new_cap = (cached_cap == 0) ? 4 : cached_cap;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_arr = seq_alloc_new_array(new_cap);
    if (cached_len > 0)
        memcpy(new_arr, cached_values, cached_len * sizeof(seq_t));
    if (cached_values)
        free_seq_array(cached_values);
    cached_values = new_arr;
    cached_cap   = new_cap;
}

/* test if a number 'n' has exactly three distinct prime substrings */
static bool has_three_prime_substrings(seq_t n)
{
    size_t len = digit_len(n);
    char *buf = (char *)malloc(len + 1);
    if (!buf)
        return false; /* out of memory – unlikely in this context */
    seq_to_str(n, buf, &len);
    buf[len] = '\0';

    seq_t primes[3];
    size_t prime_count = 0;

    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            unsigned int digit = (unsigned int)(buf[j] - '0');
            val = val * 10 + digit;
            if (val >= 2 && is_prime_big(val)) {
                bool already = false;
                for (size_t p = 0; p < prime_count; ++p)
                    if (primes[p] == val) {
                        already = true;
                        break;
                    }
                if (!already) {
                    if (prime_count == 3)
                        goto cleanup; /* more than three */
                    primes[prime_count++] = val;
                }
            }
        }
        /* early escape if already more than three */
        if (prime_count == 3)
            continue;
    }

    if (prime_count == 3) {
        free(buf);
        return true;
    }

cleanup:
    free(buf);
    return false;
}

/* ensure cached_values stores at least 'need' terms (need ≥ 1) */
static void ensure_cached(idx_t need)
{
    if (need <= 0)
        return;
    size_t target = (size_t)need;
    if (target <= cached_len)
        return;

    if (!cur_initialized) {
        cur_num = seq_from_str("2");
        cur_initialized = true;
    }

    while (cached_len < target) {
        if (has_three_prime_substrings(cur_num)) {
            // add to cache
            expand_cache(cached_len + 1);
            cached_values[cached_len++] = cur_num;
        }
        ++cur_num; /* test next number */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but guard against zero */
        return seq_from_str("0");

    ensure_cached(n);
    return cached_values[n - 1];
}
