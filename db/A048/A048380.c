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
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include "../utils/seq_from_str.h"

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
