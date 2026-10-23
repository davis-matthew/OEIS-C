/*
 * A158720  Primes p such that Floor[p/3]+p is prime.
 * https://oeis.org/A158720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A158720: primes p such that floor(p/3)+p is prime, found by incremental search */
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* For n lower than 1 the behavior is irrelevant. */
    if (n == 0) return 0;

    seq_t candidate = 2;
    idx_t count = 0;

    while (1) {
        if (isPrimeTrial(candidate)) {
            seq_t q = candidate + candidate / 3;   /* floor division is implicit */
            if (isPrimeTrial(q)) {
                ++count;
                if (count == n)
                    return candidate;
            }
        }
        candidate += 1;
    }
}
