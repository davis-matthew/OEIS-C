/*
 * A138906  a(n) is n-th prime == 1 (mod 6n).
 * https://oeis.org/A138906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime congruent to 1 modulo 6n
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* Undefined for n=0; safe fallback. */
    }

    const seq_t mod = (seq_t)n * 6;         /* 6n as seq_t */
    seq_t count   = 0;                      /* how many primes seen */
    seq_t cand    = mod + 1;                /* first candidate: 1 + 6n */

    for (;;) {
        if (is_prime_big(cand)) {
            ++count;
            if (count == (seq_t)n) {
                return cand;
            }
        }
        cand += mod;                        /* next member of the progression */
    }
}
