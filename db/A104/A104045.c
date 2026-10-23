/*
 * A104045  Numbers k such that k9 is prime and k is a multiple of ten.
 * https://oeis.org/A104045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10 * t where t is the smallest positive integer such that
 *            100*t + 9 is prime
 * Range:     u8 n <= 11, u16 n <= 1322, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset 1, so a(1) = 10 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the nth term of sequence A104045 */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 1; returning 0 is harmless */
        return 0;

    idx_t count = 0;
    uint64_t t = 1;

    /* Iterate over positive integers t, checking primality of 100*t+9 */
    while (true) {
        uint64_t cand = 100ULL * t + 9ULL;   /* 100*t + 9 */

        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)(10ULL * t);  /* k = 10*t */
        }

        ++t;
    }
}
