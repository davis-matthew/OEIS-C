/*
 * A144426  Primes of the form (p(n)-2*(n-1))/7, where p(n)=n-th prime.
 * https://oeis.org/A144426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 399, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A144426: primes of the form
   (p(i) - 2*(i-1))/7, where p(i) is the i‑th prime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 1;

    /* iterate over primes i = 1, 2, 3, … until the n‑th term is found */
    while (true) {
        uint32_t prime = get_prime(i);              /* p(i) */
        seq_t temp = (seq_t)prime -
                     (seq_t)2 * ((seq_t)i - (seq_t)1);  /* p(i)-2*(i-1) */

        /* temp must be positive and divisible by 7 */
        if (temp > 0 && temp % 7 == (seq_t)0) {
            seq_t q = temp / (seq_t)7;              /* candidate prime */

            if (is_prime_big(q)) {
                count++;                            /* found one term */
                if (count == n)
                    return q;                       /* return the n‑th term */
            }
        }
        i++;                                        /* next prime index */
    }
}
