/*
 * A162398  a(n) = the largest Fibonacci number that when represented in
 *          binary is a substring in the binary representation of n.
 * https://oeis.org/A162398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest Fibonacci number whose binary representation
 *            appears as a contiguous substring in the binary representation
 *            of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   binary_substring, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/binary_substring.h"

enum { MAX_FIB_COUNT = 93 };            /* F(94) > 2^64 - 1 */

/* Fibonacci values 1, 2, 3, 5, ... stored as uint64_t. */
static uint64_t fib_values[MAX_FIB_COUNT];
static size_t fib_count = 0;

/* Initialise the Fibonacci table once. */
static void init_fib_values(void)
{
    if (fib_count != 0)               /* already initialised */
        return;

    fib_values[0] = 1ULL;             /* F(1) */
    fib_values[1] = 2ULL;             /* F(2) */
    fib_count    = 2;

    for (size_t i = 2; i < MAX_FIB_COUNT; ++i) {
        uint64_t a = fib_values[i - 2];
        uint64_t b = fib_values[i - 1];
        if (UINT64_MAX - b < a)       /* would overflow, stop */
            break;
        fib_values[i] = a + b;
        ++fib_count;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                         /* undefined in OEIS, return 0 */
        return 0;

    init_fib_values();

    /* Binary length of n. */
    uint8_t nlen = (uint8_t)(floor_log2_uint64(n) + 1);

    /* Scan Fibonacci numbers from largest to smallest. */
    for (size_t ii = fib_count; ii-- > 0; ) {
        uint64_t fib   = fib_values[ii];
        uint8_t flen   = (uint8_t)(floor_log2_uint64(fib) + 1);

        if (flen > nlen)               /* cannot fit as substring */
            continue;

        if (binary_substring(n, nlen, fib, flen))
            return (seq_t)fib;          /* largest matching Fibonacci */
    }

    /* Fallback – should never happen (n=1 contains the 1‑bit pattern). */
    return (seq_t)1;
}
