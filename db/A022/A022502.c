/*
 * A022502  Describe the previous term! (method B - initial term is 6).
 * https://oeis.org/A022502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from OEIS A022502: describe the previous term (method B) starting from 6.
   a(1) = 6.
   For n ≥ 1, a(n+1) is produced by scanning the decimal digits of a(n) from left to right,
   grouping consecutive identical digits, and writing for each run the digit followed by
   the run length (in decimal). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* static cache for memoisation */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;  /* number of terms stored */
static size_t cache_cap      = 0;  /* allocated capacity */

/* buffers for decimal digit extraction */
static uint8_t *digit_buf    = NULL;
static size_t digit_cap      = 0;
static uint8_t *count_buf    = NULL;
static size_t count_cap      = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but harmless */

    /* initialise cache with the first term */
    if (cache_len == 0) {
        cache_cap   = 4;
        cache       = seq_alloc(cache_cap);
        cache[0]    = (seq_t)6;
        cache_len   = 1;
    }

    /* return cached value if already available */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* ensure cache capacity */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];

        /* extract digits of prev into digit_buf (least significant first) */
        size_t digits = 0;
        seq_t tmp = prev;
        while (tmp != (seq_t)0) {
            if (digits == digit_cap) {
                digit_cap = digit_cap ? digit_cap * 2 : 8;
                digit_buf = (uint8_t *)realloc(digit_buf, digit_cap);
            }
            digit_buf[digits++] = (uint8_t)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }
        /* handle the special case of prev == 0 (not expected) */
        if (digits == 0) {
            digits = 1;
            if (digit_cap == 0) {
                digit_cap = 8;
                digit_buf = (uint8_t *)malloc(digit_cap);
            }
            digit_buf[0] = 0;
        }

        /* build the next term */
        seq_t result = (seq_t)0;
        size_t pos = digits;          /* next index to process (from most significant) */
        while (pos > 0) {
            uint8_t d = digit_buf[pos - 1];
            size_t run = 1;
            pos--;
            while (pos > 0 && digit_buf[pos - 1] == d) {
                run++;
                pos--;
            }

            /* append the digit */
            result = result * (seq_t)10 + d;

            /* append the run length in decimal */
            seq_t rc = (seq_t)run;
            size_t cnt_len = 0;
            seq_t rc_tmp = rc;
            while (rc_tmp != (seq_t)0) {
                if (cnt_len == count_cap) {
                    count_cap = count_cap ? count_cap * 2 : 8;
                    count_buf = (uint8_t *)realloc(count_buf, count_cap);
                }
                count_buf[cnt_len++] = (uint8_t)(rc_tmp % (seq_t)10);
                rc_tmp /= (seq_t)10;
            }
            for (size_t j = 0; j < cnt_len; ++j) {
                result = result * (seq_t)10 + count_buf[cnt_len - 1 - j];
            }
        }

        cache[i] = result;

        /* ensure capacity for the next iteration if needed */
        if (i + 1 >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
