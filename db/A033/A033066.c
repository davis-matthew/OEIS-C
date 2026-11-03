/*
 * A033066  Numbers whose base-15 representation Sum_{i=0..m} d(i)*15^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A033066
   Numbers whose base‑15 representation
   Σ d(i)*15^i has odd d(m−i) for all odd i
   (m = most significant digit index). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static bool valid_base15(seq_t val)
{
    /* convert to base‑15 digits (least significant first) */
    unsigned char digits[256];
    size_t len = 0;
    seq_t tmp = val;
    do {
        seq_t d = tmp % (seq_t)15;
        digits[len++] = (unsigned char)d;
        tmp /= (seq_t)15;
    } while (tmp > 0);

    /* most significant digit index m */
    size_t m = len - 1;

    /* check parity condition for all odd (m‑i) */
    for (size_t j = 0; j < len; ++j) {
        if ( (((m - j) & 1U) == 1U) && ((digits[j] & 1U) == 0U) )
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset is 1 */

    /* static cache of sufficient capacity for requested n */
    enum { CACHE_CAP = 20000 };          /* enough for n ≤ 10000 */
    static seq_t cache[CACHE_CAP];
    static size_t cache_size = 0;
    static seq_t next = (seq_t)1;        /* next integer to test */

    /* generate terms until we have n of them */
    while (cache_size < (size_t)n) {
        if (valid_base15(next)) {
            cache[cache_size++] = next;
        }
        ++next;
    }
    return cache[(size_t)n - 1];
}
