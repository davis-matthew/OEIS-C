/*
 * A083984  If k is a number with exactly two distinct decimal digits, say a
 *          and b, neither of which is 0 (i.e., a member of A101594), define
 *          the self-complement of k, SC(k), to be the number obtained by
 *          replacing a with b and vice versa. E.g. SC(232233) = 323322.
 *          Sequence contains numbers n such that n and...
 * https://oeis.org/A083984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083984:  
   a(n) is the n‑th positive integer k that has exactly two distinct
   non‑zero decimal digits a and b and for which gcd(k, SC(k)) = 1,
   where SC(k) is obtained by swapping a and b.  Computation is done
   by scanning consecutive integers and caching results. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Extract decimal digits of x into buf (least‑significant first). */
static void
get_digits(seq_t x, uint8_t *buf, size_t *len)
{
    size_t i = 0;
    if (x == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        buf[i++] = d;
        x /= 10;
    }
    *len = i;
}

/* Return true iff x has exactly two distinct non‑zero decimal digits.  */
static bool
has_two_nonzero_digits(seq_t x, uint8_t *a_out, uint8_t *b_out)
{
    uint16_t mask = 0;
    size_t len;
    uint8_t buf[128];
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        if (d == 0) {
            return false;
        }
        mask |= (1U << d);
        if (__builtin_popcount(mask) > 2) {
            return false;
        }
    }
    if (__builtin_popcount(mask) != 2) {
        return false;
    }
    *a_out = 0;
    *b_out = 0;
    for (int d = 1; d <= 9; ++d) {
        if (mask & (1U << d)) {
            if (*a_out == 0) {
                *a_out = (uint8_t)d;
            } else {
                *b_out = (uint8_t)d;
            }
        }
    }
    return true;
}

/* Compute the self‑complement of x given its two digits a and b. */
static seq_t
self_complement(seq_t x, uint8_t a, uint8_t b)
{
    size_t len;
    uint8_t buf[128];
    get_digits(x, buf, &len);

    seq_t y = 0;
    /* Process digits from most‑significant to least‑significant. */
    for (size_t i = len; i-- > 0; ) {
        uint8_t d = (buf[i] == a) ? b : a;
        y = y * 10 + d;
    }
    return y;
}

/* Main sequence function with simple caching. */
seq_t
a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of values stored */
    static seq_t next_candidate = 1; /* next integer to test */

    if (n == 0) return 0;   /* safety for offset 1 */

    /* Ensure enough cached terms. */
    while (cache_len < n) {
        seq_t cand = next_candidate;
        for (;;) {
            uint8_t a_d, b_d;
            if (has_two_nonzero_digits(cand, &a_d, &b_d)) {
                seq_t sc = self_complement(cand, a_d, b_d);
                if (gcd(cand, sc) == 1) {
                    break;          /* valid term found */
                }
            }
            ++cand;
        }

        /* Grow cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;
        next_candidate = cand + 1;
    }

    return cache[n - 1];
}
