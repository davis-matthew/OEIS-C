/*
 * A059325  Numbers n such that 6n + 5 is prime.
 * https://oeis.org/A059325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 3462, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  a(n) is the n-th non‑negative integer k such that 6k+5 is prime.
 * Implementation searches k from 0 upward, testing 6k+5 for primality
 * with the supplied helper functions.  The function works for all
 * seq_t sizes supported by the build. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

static bool is_prime(seq_t v)
{
    /* Dispatch to the appropriate primality routine depending on the
       underlying size of seq_t.  seq_t can be an 8, 16, 32, 64 bit
       unsigned integer or a big‑integer class.  The helper functions are
       provided by the build and accept a seq_t argument. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return is_prime_small(v);
    else
        return is_prime_big(v);
}

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined in OEIS, return 0 for safety */
        return 0;

    idx_t found = 0;
    idx_t k = 0;     /* candidate integer */

    while (found < n) {
        seq_t val = (seq_t)6 * (seq_t)k + (seq_t)5;
        if (is_prime(val)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }

    /* Should never be reached */
    return 0;
}
