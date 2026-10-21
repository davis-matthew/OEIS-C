/*
 * A082900  a(n) = A082894(n)-A000079(n), the difference of 2^n and the number
 *          closest to it and divisible by n.
 * https://oeis.org/A082900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A082894(n) - 2^n; * A082894(n) is the multiple of n
 *            nearest to 2^n (ties rounded down)
 * Range:     i8 n <= 288, i16 n <= 9677, i32 n <= 9677, i64 n <= 9677,
 *            i128 n <= 9677, big any n
 * Helpers:   powmod2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses modular exponentiation to avoid computing huge 2^n values. */

#include <stdint.h>
#include "../utils/powmod2_uint64.h"

seq_t a(idx_t n) {
    /* n >= 1 due to offset 1 */
    uint64_t mod = (uint64_t)n;          /* modulus n */
    uint64_t r = powmod2_uint64(2U, n, mod);  /* r = 2^n mod n */

    if (r == 0)
        return 0;                       /* 2^n is already a multiple of n */

    uint64_t half = mod >> 1;          /* floor(n/2) */
    if (r <= half)
        return -(seq_t)r;               /* nearest multiple is lower: 2^n - r */
    else
        return (seq_t)(mod - r);        /* nearest multiple is higher: 2^n + (n - r) */
}
