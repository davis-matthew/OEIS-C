/*
 * A106107  Primes with minimal digit = 7.
 * https://oeis.org/A106107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 2505, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate primes composed only of the digits 7,8,9
   that contain at least one digit 7.  The primes are generated in
   increasing numeric order; the nth such prime is returned. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Recursive enumeration of all numbers of a given length whose digits are
   {7,8,9} and that contain at least one 7.  When a prime is found the
   global counter `cnt` is incremented and, if it reaches the target,
   the result is stored. */
static void enumerate(uint64_t val,
                      int pos,
                      int len,
                      bool has7,
                      seq_t *cnt,
                      idx_t target,
                      seq_t *result,
                      bool *found)
{
    if (*found) return;
    if (pos == len) {
        if (has7 && is_prime(val)) {
            ++(*cnt);
            if (*cnt == target) {
                *result = (seq_t)val;
                *found = true;
            }
        }
        return;
    }

    /* current digit */
    for (int d = 0; d < 3; ++d) {
        int digit = (d == 0) ? 7 : ((d == 1) ? 8 : 9);
        /* no trailing 8 (even last digit) */
        if (pos == len - 1 && digit == 8) continue;
        enumerate(val * 10ULL + (uint64_t)digit,
                  pos + 1,
                  len,
                  has7 || (digit == 7),
                  cnt,
                  target,
                  result,
                  found);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset: undefined */
    seq_t   count  = 0;             /* number of primes found */
    bool    found  = false;
    seq_t   result = 0;

    /* enumerate increasing lengths until the target prime is found */
    for (int len = 1; !found; ++len) {
        enumerate(0, 0, len, false, &count, n, &result, &found);
    }

    return result;
}
