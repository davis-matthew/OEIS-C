/*
 * A071875  Decimal expansion of the eighth (of 10) decimal selvage number;
 *          the n-th digit of a decimal selvage number, x, is equal to the
 *          tenths digit of n*x.
 * https://oeis.org/A071875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A071875 – Decimal expansion of the eighth decimal selvage number.
    Formula (OEIS notation) :

    a(0) = 7
    for n >= 1
        a(n) = floor( 10 * (n+1) * x ) mod 10
    with    x = Σ_{k>=0} a(k) / 10^{k+1}   (the decimal expansion of the number).

    The implementation generates the digits sequentially.  It keeps the
    already computed digits in a small array of bytes (one byte per decimal
    digit).  For each new term it multiplies this digit string by (n+1)
    using elementary base‑10 arithmetic and records only the most
    significant resulting digit as the next term.  No large integers,
    floating point arithmetic or dynamic arrays beyond a simple byte buffer
    are required, so the routine works for any seq_t type (unsigned 8/16/32/64
    bits or an arbitrary‑precision class).  It runs in O(n) time and uses
    O(n) memory (about 10 kB for n=10 000).  All arithmetic on seq_t
    values is avoided – only the final value is cast to seq_t.  C++17
    compilation is also supported because only C style headers and
    malloc are used. */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Special case for the first digit */
    if (n == 0u)
        return (seq_t)7u;

    /* Allocate a byte array large enough to hold all digits computed
       up to the requested index.  The array is indexed such that
       digits[0] is the most significant digit (a(0)), digits[len-1]
       is the least significant digit of the current number. */
    size_t needed = (size_t)n + 2;          /* +2 for safety */
    uint8_t *digits = (uint8_t *)malloc(needed * sizeof(uint8_t));
    if (!digits)
        return (seq_t)0u;                  /* abort on allocation failure */

    digits[0] = 7u;                         /* a(0) */
    size_t len = 1u;                        /* current length of digits */

    seq_t result = 0u;                      /* will hold a(n) */

    /* Generate digits one by one until the desired index is reached. */
    for (size_t k = 1u; k <= n; ++k)
    {
        /* Multiply the current number by (k+1) in base 10.
           Only the most significant digit of the product is needed
           for the next sequence value. */
        int factor = (int)(k + 1u);          /* small enough for int */
        int carry = 0;
        int ak = 0;                          /* next digit a(k) */

        for (int i = (int)len - 1; i >= 0; --i)
        {
            int prod = (int)digits[i] * factor + carry;
            int digit = prod % 10;
            if (i == 0)
                ak = digit;                 /* most significant result digit */
            carry = prod / 10;
        }

        /* Store the new digit and update length. */
        digits[len++] = (uint8_t)ak;

        if (k == n)
            result = (seq_t)ak;
    }

    free(digits);
    return result;
}
