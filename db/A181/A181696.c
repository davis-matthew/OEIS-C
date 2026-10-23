/*
 * A181696  Number of iterations p -> ap+b needed to get a composite number,
 *          starting with p=prime(n) and the least even a>0 and odd b<a, b>0
 *          such that a*prime(n)+b is prime.
 * https://oeis.org/A181696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of iterations needed to reach a composite when
 *            starting at the n‑th prime p, applying the rule x → a*x + b
 *            where a is the smallest even integer > 0 and b the smallest odd
 *            integer 0 < b < a such that (a p + b) is prime
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 65, u64 n <= 168, u128 n <= 168,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Prime to start from */
    seq_t p = (seq_t)get_prime(n);

    /* Search for the least even a and odd b with a*p+b prime */
    idx_t a_val = 2;        /* smallest even integer > 0 */
    for (;; a_val += 2) {   /* iterate over even a 2,4,6,… */
        for (idx_t b_val = 1; b_val < a_val; b_val += 2) { /* odd b < a */
            seq_t cand = (seq_t)a_val * p + b_val;
            if (is_prime_big(cand)) {
                seq_t A = (seq_t)a_val;  /* store a as seq_t for multiplication */
                seq_t B = (seq_t)b_val;  /* store b as seq_t for multiplication */

                /* Count iterations until a composite is reached */
                seq_t count = 0;
                seq_t cur = p;
                while (is_prime_big(cur)) {
                    cur = A * cur + B;
                    ++count;
                }
                return count;
            }
        }
    }
    /* This point never reached – the loops always find a valid pair */
    return 0;
}
