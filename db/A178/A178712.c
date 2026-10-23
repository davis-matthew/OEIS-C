/*
 * A178712  Numbers n such that the sum of subfactorial of the decimal digits
 *          of n is prime.
 * https://oeis.org/A178712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A178712
 *  a(n) = the nth number for which the sum of subfactorials of its
 *       decimal digits is prime.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h> /* for bool */
#include "../utils/is_prime_small.h" /* assumes is_prime_small(seq_t) exists */

seq_t a(idx_t n) {
    /*  Static cache of discovered terms */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t count    = 0;

    /*  When cache empty initialise storage  */
    if (capacity == 0) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0; /* memory allocation failure */
    }

    /*  Generate terms until we have at least n of them  */
    static idx_t current = 1;   /* candidate number to test, start from 1 */

    /*  Subfactorial values for digits 0-9  */
    static const unsigned SUB[10] = {
        1,   /* !0  */
        0,   /* !1  */
        1,   /* !2  */
        2,   /* !3  */
        9,   /* !4  */
        44,  /* !5  */
        265, /* !6  */
        1854,/* !7  */
        14833,/* !8 */
        133496/* !9 */
    };

    while (count < n) {
        /*  Compute the sum of subfactorials of the decimal digits of current  */
        uint64_t sum = 0;
        uint64_t t   = current;
        if (t == 0) { /* not needed but keeps logic clear */
            sum = SUB[0];
        } else {
            while (t > 0) {
                unsigned d = (unsigned)(t % 10);
                sum += SUB[d];
                t   /= 10;
            }
        }

        /*  Check primality of the sum  */
        if (is_prime_small((seq_t)sum)) {
            /*  Ensure cache capacity  */
            if (count == capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (!tmp) return 0; /* memory allocation failure */
                cache = tmp;
            }
            cache[count] = (seq_t)current;
            count++;
        }
        current++;  /*  next candidate  */
    }

    return cache[n - 1];
}
