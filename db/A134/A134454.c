/*
 * A134454  Numbers m such that the digital root in balanced ternary
 *          representation equals +1.
 * https://oeis.org/A134454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 942 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute Numbers m such that the digital root in balanced ternary
    representation equals +1.
    The sequence is built by scanning successive integers and testing
    the digital‑root property.  A small cache is kept so that the
    function works for arbitrary orders of calls.
    Formula: a(n) = m where m is the n‑th number whose balanced ternary
    digital root is +1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Sum the digits of a number in balanced ternary.
   The function uses a carry mechanism that turns each ternary digit
   2 into –1 with a carry to the next higher position.  The sum
   is returned as a signed int because it is bounded by the number
   of ternary digits (~log_3 m). */
static int bal_digit_sum(seq_t n)
{
    int carry = 0;
    int sum   = 0;
    while (n > 0 || carry) {
        int r = (int)(n % 3);
        n /= 3;
        int s = r + carry;
        int digit;
        if (s == 0) {                 /* 0 -> 0, no carry */
            digit = 0;
            carry = 0;
        } else if (s == 1) {          /* 1 -> 1, no carry */
            digit = 1;
            carry = 0;
        } else if (s == 2) {          /* 2 -> –1 with carry 1 */
            digit = -1;
            carry = 1;
        } else {                      /* s == 3 -> 0 with carry 1 */
            digit = 0;
            carry = 1;
        }
        sum += digit;
    }
    return sum;
}

/* Test whether a number has a balanced ternary digital root of +1. */
static bool bal_root_plus_one(seq_t m)
{
    int r = bal_digit_sum(m);
    /* Repeat digit summation until the value is in {-1,0,1}. */
    while (r < -1 || r > 1) {
        bool neg = r < 0;
        int abs_r = neg ? -r : r;
        int next = bal_digit_sum((seq_t)abs_r);
        if (neg) next = -next;
        r = next;
    }
    return r == 1;
}

/* Sequence function: a(idx_t n) */
seq_t a(idx_t n)
{
    /* Cache of generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_val = 0;   /* largest term already stored */

    /* Allocate cache on first call. */
    if (cache == NULL) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;   /* unlikely, but defensive */
    }

    /* Return cached value if available. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend cache until reaching the requested index. */
    seq_t cand = last_val;           /* start scanning after last value */
    while (cache_len < n) {
        cand = cand + 1;             /* next integer to test */
        if (bal_root_plus_one(cand)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0; /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
            last_val = cand;
        }
    }

    return cache[n - 1];
}
