/*
 * A048380  Digits d in decimal expansion of n replaced with d-th prime
 *          (replacing each 0 digit with a 1).
 * https://oeis.org/A048380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digits d in decimal expansion of n replaced with d‑th
 *            prime (replacing each 0 digit with a 1)
 * Range:     u8 n <= 24, u16 n <= 454, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 982 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static const char *prime_map[10] = {
    "1",  /* 0 → 1 */
    "2",  /* 1 → 2 */
    "3",  /* 2 → 3 */
    "5",  /* 3 → 5 */
    "7",  /* 4 → 7 */
    "11", /* 5 → 11 */
    "13", /* 6 → 13 */
    "17", /* 7 → 17 */
    "19", /* 8 → 19 */
    "23"  /* 9 → 23 */
};

seq_t a(idx_t n) {
    char decbuf[32];
    /* Convert n to decimal string */
    snprintf(decbuf, sizeof(decbuf), "%llu", (unsigned long long)n);

    /* Build the output string by replacing each digit */
    char outbuf[64];
    int outpos = 0;
    for (char *p = decbuf; *p; ++p) {
        int d = *p - '0';
        const char *m = prime_map[d];
        for (; *m; ++m) {
            outbuf[outpos++] = *m;
        }
    }
    outbuf[outpos] = '\0';

    return seq_from_str(outbuf);
}
