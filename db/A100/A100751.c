/*
 * A100751  Concatenate all natural numbers starting with 1 in binary like
 *          this 11011100101110111100010011010..., then a(n) = the number
 *          formed from the next n digits (by dropping leading zeros). 1, 10,
 *          111, 0010, 11101, 111000, ... 1, 10, 111, 10, 11101, 111000, ...
 * https://oeis.org/A100751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the decimal integer whose decimal digits lie in the next
 *            n bits of the infinite binary stream obtained by concatenating
 *            the binary representations of the natural numbers starting with
 *            1; leading zeros are automatically dropped by the decimal
 *            representation
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Global storage for the binary stream.  The buffer is extended
   as needed and retains all bits requested for any call of a(). */
static uint8_t *bits          = NULL;   /* bit buffer, most‐significant
                                           bit first in each byte   */
static size_t bits_capacity   = 0;      /* allocated byte count       */
static uint64_t bits_used     = 0;      /* number of bits stored      */
static uint64_t next_num      = 1;      /* next natural number to
                                           encode in the stream      */

/* Ensure that at least 'upto' bits (1‑based count) are available
   in the global buffer.  After this call bits_used >= upto. */
static void
ensure_bits(uint64_t upto)
{
    while (bits_used < upto) {
        /* Encode the next natural number in binary. */
        uint64_t num = next_num++;
        int len = 0;
        uint64_t t = num;
        while (t) { ++len; t >>= 1; }
        if (len == 0) len = 1;   /* only if num == 0, which never
                                      happens for natural numbers   */

        /* Grow the buffer if necessary. */
        size_t needed_bytes = (bits_used + len + 7) / 8;
        if (needed_bytes > bits_capacity) {
            size_t new_capacity = needed_bytes * 2 + 64;
            uint8_t *tmp = (uint8_t *)realloc(bits, new_capacity);
            if (!tmp) abort();    /* memory exhaustion          */
            memset(tmp + bits_capacity, 0, new_capacity - bits_capacity);
            bits = tmp;
            bits_capacity = new_capacity;
        }

        /* Pack the bits of 'num' into the buffer. */
        for (int i = 0; i < len; ++i) {
            uint8_t b = (num >> (len - 1 - i)) & 1;
            size_t pos = bits_used + i;                /* 0‑based         */
            bits[pos / 8] |= b << (7 - (pos % 8));
        }
        bits_used += len;
    }
}

/* Return the bit (0 or 1) at zero‑based position 'pos'. */
static uint8_t
get_bit(uint64_t pos)
{
    return (bits[pos / 8] >> (7 - (pos % 8))) & 1;
}

/* Primary API function.  n is one‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence is undefined for n==0 */

    /* Starting position (0‑based) in the bit stream: sum_{i=1}^{n-1} i. */
    idx_t m = (idx_t)((((seq_t)n - 1) * (seq_t)n) / 2);
    uint64_t start0 = (uint64_t)m;
    uint64_t end0   = start0 + (uint64_t)n - 1;  /* inclusive end */

    /* Ensure the global buffer contains all required bits. */
    ensure_bits(end0 + 1);                         /* 1‑based count */

    /* Assemble the required block into the result,
       treating each bit as a decimal digit. */
    seq_t value = 0;
    for (uint64_t p = start0; p <= end0; ++p) {
        value = value * (seq_t)10 + (seq_t)get_bit(p);
    }

    return value;
}
