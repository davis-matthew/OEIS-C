/*
 * A070972  Length of longest run of consecutive 1's in binary expansion of
 *          3^n (A004656).
 * https://oeis.org/A070972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Longest run of consecutive 1 bits in the binary representation of 3^n */
#include <stdint.h>
#include <stdlib.h>

/* Multiply the big integer in words[0..len-1] by 3.
   words uses base 2^64 with little‑endian order. */
static void mul3(uint64_t *words, size_t *len)
{
    __uint128_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        __uint128_t prod = (__uint128_t)words[i] * 3 + carry;
        words[i] = (uint64_t)prod;
        carry    = prod >> 64;
    }
    if (carry) {
        words[*len] = (uint64_t)carry;
        ++*len;
    }
}

seq_t a(idx_t n)
{
    /* Allocate a buffer large enough for 3^n.
       The number of bits never exceeds 2*n, so we need at most n/32 words. */
    static uint64_t *buf = NULL;
    static size_t buf_cap = 0;

    size_t needed = n / 32 + 5;   /* small safety margin */
    if (buf_cap < needed) {
        uint64_t *tmp = (uint64_t *)realloc(buf, needed * sizeof(uint64_t));
        if (!tmp) abort(); /* memory allocation failed */
        buf      = tmp;
        buf_cap  = needed;
    }

    size_t len = 1;
    buf[0] = 1;                    /* 3^0 = 1 */

    for (idx_t i = 0; i < n; ++i)
        mul3(buf, &len);

    /* Scan all bits (including trailing zeros in the most‑significant word)
       -- trailing zeros do not affect the maximum run length. */
    int max_run = 0, run = 0;
    for (size_t wi = 0; wi < len; ++wi) {
        uint64_t w = buf[wi];
        for (int bi = 0; bi < 64; ++bi) {
            if (w & ((uint64_t)1 << bi)) {
                ++run;
                if (run > max_run) max_run = run;
            } else {
                run = 0;
            }
        }
    }

    return (seq_t)max_run;
}
