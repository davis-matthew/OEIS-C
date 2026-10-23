/*
 * A138692  Numbers of the form 86+p^2 (where p is a prime).
 * https://oeis.org/A138692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138692: Numbers of the form 86 + p^2 where p is a prime.
   Formula: a(n) = 86 + (p_n)^2,   p_n = nth prime (n >= 1).  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return the nth term of OEIS sequence A138692. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    /* Rough upper bound for the nth prime: 20*n+100.
       For n <= 10000 this bound is > 104729 (the 10000th prime). */
    idx_t limit = 20 * n + 100;
    size_t sz = (size_t)limit + 1;

    /* Allocate a boolean array for the sieve (C++ needs an explicit cast). */
    bool *comp = (bool *)calloc(sz, sizeof(bool));
    if (!comp)
        return (seq_t)0;  /* allocation failure */

    idx_t count = 0;
    idx_t prime = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            /* i is prime */
            ++count;
            prime = i;
            if (count == n) {
                free(comp);
                seq_t sq = (seq_t)prime * (seq_t)prime;
                return sq + (seq_t)86;
            }

            /* Mark multiples of i as composite */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    comp[j] = true;
            }
        }
    }

    /* Should not happen for valid n with the chosen bound. */
    free(comp);
    return (seq_t)0;
}
