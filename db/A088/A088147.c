/*
 * A088147  n-th prime rotated one binary place to the left.
 * https://oeis.org/A088147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = rotate left by one bit of the n‑th prime
 * Range:     u8 n <= 54, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   rotate_left_one_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             a(1)=1, a(2)=3, a(3)=3, a(4)=7, …                            */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/rotate_left_one_bit.h"

#define MAX_PRIME_LIMIT 200000      /* > p(10000) = 104729              */
#define MAX_PRIMES       20000      /* number of primes up to MAX_LIMIT */

static uint8_t  is_composite[MAX_PRIME_LIMIT + 1];
static idx_t    primes[MAX_PRIMES];
static idx_t    prime_count = 0;

/* ----------------------------------------------------------------------- */
/* Simple sieve of Eratosthenes, executed once on the first call to a().    */
/* ----------------------------------------------------------------------- */
static void init_primes(void)
{
    if (prime_count != 0)  return;                  /* already done      */

    for (idx_t i = 0; i <= MAX_PRIME_LIMIT; ++i) is_composite[i] = 0;

    is_composite[0] = is_composite[1] = 1;
    for (idx_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_composite[j] = 1;

    idx_t pos = 0;
    for (idx_t i = 2; i <= MAX_PRIME_LIMIT && pos < MAX_PRIMES; ++i)
        if (!is_composite[i]) primes[pos++] = i;

    prime_count = pos;
}

/* ----------------------------------------------------------------------- */
/* Rotate left by one bit.  The rotation keeps only the bits that are        */
/* present in the binary representation of x (no leading zeros).           */

/* ----------------------------------------------------------------------- */
/* The required function.  It guarantees that n≥1 is satisfied.            */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but we return 0 */

    init_primes();                 /* ensure primes are ready */

    /* If n exceeds the precomputed primes, extend the sieve */
    if (n > prime_count) {
        /* Increase limit until enough primes are found.  Safe for small n. */
        idx_t new_limit = MAX_PRIME_LIMIT;
        while (prime_count < n) {
            new_limit *= 2;
            if (new_limit > 10000000) break;   /* safeguard for very large n */
            for (idx_t i = 0; i <= new_limit; ++i) is_composite[i] = 0;
            is_composite[0] = is_composite[1] = 1;
            for (idx_t i = 2; i * i <= new_limit; ++i)
                if (!is_composite[i])
                    for (idx_t j = i * i; j <= new_limit; j += i)
                        is_composite[j] = 1;
            idx_t pos = 0;
            for (idx_t i = 2; i <= new_limit && pos < MAX_PRIMES; ++i)
                if (!is_composite[i]) primes[pos++] = i;
            prime_count = pos;
        }
    }

    seq_t p = (seq_t)primes[n - 1];
    return rotate_left_one_bit(p);
}
