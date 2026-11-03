/*
 * A051808  Numbers with nonzero multiplicative digital root 6.
 * https://oeis.org/A051808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051808: Numbers with nonzero multiplicative digital root 6
   Formula: brute‑force search for numbers whose multiplicative digital
   root equals 6, stored in ascending order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* compute multiplicative digital root of v, ignoring zero digits */
static seq_t mdroot(seq_t v)
{
    seq_t x = v;
    while (x >= 10) {          /* while more than one digit     */
        seq_t prod = 1;
        seq_t t = x;
        while (t > 0) {
            seq_t d = t % 10;  /* extract last digit           */
            if (d != 0)
                prod *= d;    /* multiply non‑zero digits      */
            t /= 10;          /* remove last digit            */
        }
        x = prod;
    }
    return x;
}

/* static cache: maximum number of terms we are willing to precompute
   (sufficient for n up to at least 100 000). Adjust if higher values
   are needed. */
#define MAX_CACHE 200000

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;          /* number of terms stored */
static idx_t next_num = 1;            /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1            */
        return 0;

    /* enlarge cache if necessary (unlikely for 10000th term) */
    if (n > MAX_CACHE)
        return 0;              /* fallback: sequence too long for cache */

    /* continue scanning until we have at least n terms */
    while (cache_len < n) {
        if (mdroot(next_num) == 6)
            cache[cache_len++] = next_num;
        ++next_num;
    }

    return cache[n - 1];
}
