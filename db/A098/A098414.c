/*
 * A098414  Middle members q of prime triples (p,q,r) with p<q<r=p+6.
 * https://oeis.org/A098414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th middle member q of prime triples (p,q,r) with p<q<r=p+6. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 7, a(2) = 11, a(3) = 13, ... */
    if (n == 0) return 0;   /* undefined for offset 0 */

    /* Initial sieve limit – grows if needed. */
    idx_t limit = 600 * n;
    if (limit < 100) limit = 100;

    while (1) {
        unsigned char *sieve = (unsigned char *)calloc((size_t)(limit + 1), 1);
        if (!sieve) return 0;                     /* allocation failure */

        sieve[0] = sieve[1] = 1;                  /* 0 and 1 are not prime */

        /* Classic sieve of Eratosthenes */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (sieve[i]) continue;
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = 1;
        }

        /* Scan for prime triples and count middle members. */
        idx_t count = 0;
        idx_t answer = 0;
        for (idx_t p = 5; p + 6 <= limit; ++p) {
            if (sieve[p]) continue;               /* p is not prime */
            if (!sieve[p + 6]) {                  /* p+6 is prime */
                idx_t q = 0;
                if (!sieve[p + 2]) q = p + 2;     /* q = p+2 */
                else if (!sieve[p + 4]) q = p + 4;/* q = p+4 */
                if (q) {
                    ++count;
                    if (count == n) {
                        answer = q;
                        break;
                    }
                }
            }
        }

        free(sieve);

        if (answer != 0) return (seq_t)answer;

        /* Not enough terms yet – double the limit and retry. */
        limit *= 2;
        if (limit < 100) limit = 100;            /* safety guard */
    }
}
