/*
 * A089987  Primes in the concatenation of consecutive numbers beginning with
 *          2.
 * https://oeis.org/A089987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   digits_of, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089987: primes obtained by concatenating consecutive integers
   starting with 2.
   The function builds the concatenated integer incrementally, tests each
   new prefix for primality, and caches the primes in a static array so
   that repeated calls require no recomputation.  No closed formula is known
   for this sequence. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_of.h"   /* int digits_of(idx_t x);                      */
#include "../utils/pow10.h"        /* seq_t pow10(idx_t exp);                     */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n);                 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* offset 1 – not used */

    /* Static state retained across calls */
    static seq_t *primes = NULL;            /* cached primes                            */
    static size_t cap = 0;                  /* capacity of the array                    */
    static size_t count = 0;                /* number of primes stored                   */
    static idx_t cur_m = 1;                 /* last integer appended to the number       */
    static seq_t cur_val = 0;               /* current concatenated value                */

    /* Allocate initial array if first call */
    if (!primes) {
        cap = 128;
        primes = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Ensure array can hold at least 'count+1' elements */
    if (count >= cap) {
        cap <<= 1;
        primes = (seq_t*)realloc(primes, cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n of them */
    while (count < n) {
        /* Extend the concatenation by the next integer */
        ++cur_m;
        int d = digits_of(cur_m);                       /* number of decimal digits */
        seq_t pow10d = pow10((idx_t)d);
        cur_val = cur_val * pow10d + (seq_t)cur_m;

        /* Record the new prime if detected */
        if (is_prime_big(cur_val)) {
            if (count >= cap) {                      /* just in case                 */
                cap <<= 1;
                primes = (seq_t*)realloc(primes, cap * sizeof(seq_t));
            }
            primes[count++] = cur_val;
        }
    }

    return primes[n - 1];
}
