/*
 * A065192  Record entries in A065191.
 * https://oeis.org/A065192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n-th record value of A065191 (search: scan A065191(m) for
 *            m = 1, 2, ... and keep the new maxima)
 * Range:     u8 n <= 15, u16 n <= 69, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 66 terms through n = 77 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* A065191(m), computed in native 64-bit arithmetic (the values are small) */
static uint64_t A065191_a_u64(uint64_t n)
{
    if (n == 0)
        return 0;
    uint64_t pos = n;
    for (uint64_t k = n; k > 1; --k) {
        if (pos < k)
            continue;
        uint64_t r = (pos - k) % (2 * k);
        if (r < k)
            pos += k;
        else
            pos -= k;
    }
    return pos;
}

seq_t a(idx_t n)
{
    static uint64_t *recs = NULL;
    static size_t cap = 0;
    static idx_t count = 0;      /* records found so far */
    static uint64_t next_m = 1;  /* next A065191 index to examine */
    static uint64_t best = 0;

    if (n == 0)
        return 0;
    while (count < n) {
        uint64_t v = A065191_a_u64(next_m);
        next_m++;
        if (count == 0 || v > best) {
            best = v;
            if ((size_t)count >= cap) {
                size_t nc = cap ? cap * 2 : 128;
                uint64_t *p = (uint64_t *)realloc(recs, nc * sizeof(uint64_t));
                if (!p)
                    return 0;
                recs = p;
                cap = nc;
            }
            recs[count] = v;
            count++;
        }
    }
    return (seq_t)recs[n - 1];
}
