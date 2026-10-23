/*
 * A037426  Positive numbers having the same set of digits in base 4 and base
 *          8.
 * https://oeis.org/A037426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 632, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth positive integer whose set of digits in base 4
   equals its set of digits in base 8 (digits in base 8 are restricted to {0,1,2,3}). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* test whether a uint64_t number x satisfies the digit‑set equality */
static bool same_digit_set(uint64_t x)
{
    /* digits in base 4 */
    unsigned mask4 = 0;
    uint64_t t = x;
    do {
        unsigned d = (unsigned)(t % 4);
        mask4 |= 1u << d;
        t /= 4;
    } while (t != 0);

    /* digits in base 8 – must be < 4, and build its mask */
    unsigned mask8 = 0;
    t = x;
    do {
        unsigned d = (unsigned)(t % 8);
        if (d >= 4)           /* base‑8 digit 4–7 disallowed */
            return false;
        mask8 |= 1u << d;
        t /= 8;
    } while (t != 0);

    return mask4 == mask8;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard, sequence starts at 1 */

    /* static state for memoisation */
    static uint64_t *seq = NULL;          /* holds the found numbers, stored as uint64_t */
    static size_t cap = 0;                /* current allocation size (in elements) */
    static size_t len = 0;                /* number of elements stored */
    static uint64_t next_start = 1;       /* next candidate to test */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
    }

    while (len < (size_t)n) {
        /* find next valid number */
        while (true) {
            if (same_digit_set(next_start)) {
                if (len + 1 > cap) {          /* grow backing array */
                    size_t new_cap = cap ? cap * 2 : 8;
                    uint64_t *tmp = (uint64_t *)realloc(seq, new_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    seq = tmp;
                    cap = new_cap;
                }
                seq[len++] = next_start;
                break;
            }
            next_start += 1;
        }
        next_start += 1;   /* advance for next search iteration */
    }

    return (seq_t)seq[n - 1];   /* cast to the sequence element type */
}
