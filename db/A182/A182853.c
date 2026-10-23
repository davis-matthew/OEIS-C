/*
 * A182853  Squarefree composite integers and powers of squarefree composite
 *          integers.
 * https://oeis.org/A182853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A182853: a(n) = n-th number that is a squarefree composite
 * or a perfect power of a squarefree composite. */

#include <stdbool.h>

static bool is_good(seq_t x)
{
    if (x < 6) return false;           /* smallest term is 6 */

    seq_t remaining = x;
    seq_t p = 2;
    int distinct = 0;                   /* number of distinct prime factors */
    seq_t firstExp = 0;                 /* exponent of the first prime factor */
    bool allEqual = true;               /* flag whether all exponents are equal */

    /* factorization */
    while (p <= remaining / p) {       /* use division to avoid overflow */
        if (remaining % p == 0) {
            distinct++;
            seq_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                exp++;
            }
            if (distinct == 1)
                firstExp = exp;
            else if (firstExp != exp)
                allEqual = false;
        }
        p += (p == 2 ? 1 : 2);         /* iterate over odd numbers after 2 */
    }

    /* remaining > 1 is a prime factor with exponent 1 */
    if (remaining > 1) {
        distinct++;
        if (distinct == 1)
            firstExp = 1;
        else if (firstExp != 1)
            allEqual = false;
    }

    if (distinct < 2) return false;   /* not composite or not enough primes */
    return allEqual;                  /* squarefree composite or power thereof */
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t val = 6; ; ++val) {    /* start at the first term */
        if (is_good(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
