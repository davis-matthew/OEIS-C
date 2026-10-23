/*
 * A103630  Numbers k such that the string 235k is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A103630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103630: numbers k such that the decimal concatenation 235k is a prime */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t k = 0;          /* candidate k */
    uint64_t pow10 = 10;  /* 10^d, where d = number of decimal digits of k */
    idx_t cnt = 0;        /* how many valid k values found */

    while (cnt < n) {
        ++k;                         /* next candidate */
        if ((uint64_t)k == pow10)    /* digit count increased */
            pow10 *= 10;
        uint64_t m = 235 * pow10 + (uint64_t)k; /* form 235k */
        if (is_prime(m))
            ++cnt;
    }
    return k;
}
