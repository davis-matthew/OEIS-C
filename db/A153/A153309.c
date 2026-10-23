/*
 * A153309  Numbers k such that 3*k + 1 is not prime.
 * https://oeis.org/A153309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A153309 – Numbers k such that 3*k + 1 is not prime (a(1)=0). */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Scan non‑negative integers k until the n‑th one with
       (3*k + 1) composite (or not prime) is found. */
    seq_t k = 0;
    idx_t count = 0;

    while (1) {
        seq_t val = (seq_t)3 * k + 1;        /* 3*k + 1 */
        if (!is_prime_big(val)) {            /* false for 1 and composite numbers */
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
