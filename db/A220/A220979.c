/*
 * A220979  a(n) = 5^(4n+2) - 5^(3n+2) + 3 * 5^(2n+1) - 5^(n+1) + 1: the left
 *          Aurifeuillian factor of 5^(10n+5) - 1.
 * https://oeis.org/A220979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(4n+2) – 5^(3n+2) + 3·5^(2n+1) – 5^(n+1) + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: fast exponentiation for seq_t with a 64‑bit base */
static inline seq_t pow_seq_u64(uint64_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1U)
            result *= b;
        b *= b;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute the required powers of 5 */
    seq_t p1 = pow_seq_u64(5, 4 * n + 2);   /* 5^(4n+2)  */
    seq_t p2 = pow_seq_u64(5, 3 * n + 2);   /* 5^(3n+2)  */
    seq_t p3 = pow_seq_u64(5, 2 * n + 1);   /* 5^(2n+1)  */
    seq_t p4 = pow_seq_u64(5, n + 1);       /* 5^(n+1)   */

    /* Assemble the linear combination */
    seq_t result = p1;
    result -= p2;
    result += 3 * p3;
    result -= p4;
    result += (seq_t)1;

    return result;
}
