/*
 * A032719  Numbers k such that k prefixed by '2' and followed by '7' is
 *          prime.
 * https://oeis.org/A032719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k (k ≥ 1) such that the * concatenated
 *            number 2∘k∘7 is prime
 * Range:     u8 n <= 1, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper isPrimeGeneric which is defined in
 * "../utils/isPrimeGeneric.h". */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, guard against accidental 0 */
        return 0;

    idx_t  k          = 1;      /* current candidate k       */
    idx_t  next_mul10 = 10;     /* next threshold where digit count increases */
    seq_t  pow10d1    = 100;    /* 10^(digits(k)+1), starts with 10^2 = 100 */

    /* Count of primes found so far. */
    idx_t  count = 0;

    while (count < n) {
        /* Update digit count when k reaches 10, 100, 1000, ... */
        if (k == next_mul10) {
            next_mul10 *= 10;     /* next threshold          */
            pow10d1  *= 10;      /* increase 10^(d+1) factor */
        }

        /* Construct N = 2*10^(d+1) + k*10 + 7. */
        seq_t N = 2 * pow10d1 + (seq_t)k * 10 + 7;

        if (isPrimeGeneric(N)) {
            ++count;              /* found a new term of the sequence */
            if (count == n)
                return k;           /* return the n-th term */
        }

        ++k; /* consider next candidate k */
    }

    /* The loop always returns; this line is never reached but
     * follows the compiler's requirement that the function returns
     * a value of type seq_t. */
    return 0;
}
