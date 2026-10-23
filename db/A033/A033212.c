/*
 * A033212  Primes congruent to 1 or 19 (mod 30).
 * https://oeis.org/A033212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 1 or 19 (mod 30)
 * Range:     u8 n <= 12, u16 n <= 1611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the value by sieving until enough such primes are found. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------- */
/*  a : index → prime  (external linkage)                        */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n==0 is outside the OEIS range */

    /* Rough upper bound; usually enough for the first n special primes. */
    idx_t limit = n * 30 + 100;

    while (true) {
        /* Allocate a boolean array using calloc; cast so it works in C++ */
        unsigned char *marked = (unsigned char *)calloc((size_t)(limit + 1), 1);
        if (!marked) return 0;                         /* allocation failure */

        idx_t count = 0;  /* number of special primes found so far */

        for (idx_t i = 2; i <= limit; ++i) {
            if (!marked[i]) {  /* i is prime */
                if ((i % 30 == 1) || (i % 30 == 19)) {
                    ++count;
                    if (count == n) {
                        free(marked);
                        return (seq_t)i;   /* desired prime */
                    }
                }
                /* Mark multiples of i, avoiding overflow */
                if (i <= limit / i) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        marked[j] = 1;
                }
            }
        }
        free(marked);          /* not enough special primes; enlarge range */
        limit *= 2;            /* double the limit and retry */
    }
}
