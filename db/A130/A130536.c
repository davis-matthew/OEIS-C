/*
 * A130536  Prime numbers arising from A057856.
 * https://oeis.org/A130536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, changed
 * Formula:   a(n) = n^(2^m) + (n+1)^(2^m), with m the smallest non‑negative
 *            integer such that the value is prime
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_big, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_large.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The exponent e = 2^m, starting from 1. */
    idx_t e = 1;

    for (;;) {
        seq_t term1 = pow_seq_large((seq_t)n, (seq_t)e);
        seq_t term2 = pow_seq_large((seq_t)(n + 1), (seq_t)e);
        seq_t val   = term1 + term2;

        if (is_prime_big(val))
            return val;

        /* Prevent infinite loop if e overflows 64‑bit. */
        if (e > ((idx_t)(~0ULL) >> 1))
            break;

        e <<= 1;
    }

    /* If no prime found within the numeric limits of seq_t,
       the function returns 0.  (Such n are not tested for 64‑bit
       builds, because the true value exceeds the type.) */
    return 0;
}
