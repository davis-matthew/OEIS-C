/*
 * A201718  Primes of the form 3*m^2 - 7.
 * https://oeis.org/A201718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 2664, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes  p such that 3*m*m - 7 is prime, i.e. p = 3*m² - 7. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for offset handling */

    static seq_t  *seq     = NULL;
    static size_t  count   = 0;            /* number of primes already found */
    static size_t  capacity = 0;
    static idx_t   next_m = 1;            /* the next m to test (starts from 1) */

    /* Keep generating until we have at least n primes */
    while (count < (size_t)n)
    {
        ++next_m;
        uint64_t m   = (uint64_t)next_m;
        uint64_t val = 3ULL * m * m - 7ULL;   /* p = 3*m^2 - 7 */

        if (is_prime((idx_t)val))
        {
            if (count >= capacity)
            {
                size_t new_cap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* out‑of‑memory */
                seq = tmp;
                capacity = new_cap;
            }
            seq[count++] = (seq_t)val;
        }
    }

    return seq[n - 1];
}
