/*
 * A040969  Primes p such that x^5 = 12 has no solution mod p.
 * https://oeis.org/A040969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): nth prime p such that the congruence x^5 ≡ 12 (mod p) has no
   solution.  Equivalently: p ≡ 1 (mod 5) and 12^((p-1)/5) ≠ 1 (mod p). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined offset */

    /* Initial limit guess – enough for n ≤ 10000.  If not enough,
       the limit is doubled. */
    idx_t limit = n * 15 + 100;
    while (1) {
        /* Sieve of Eratosthenes up to 'limit' */
        bool *comp = (bool *)malloc((size_t)(limit + 1));
        if (!comp) return 0; /* allocation failure */
        memset(comp, 0, (size_t)(limit + 1));

        idx_t count = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!comp[i]) {
                /* i is prime */
                if (i % 5U == 1U) {
                    seq_t val = mod_pow((seq_t)12, (i - 1) / 5U, (seq_t)i);
                    if (val != (seq_t)1) {
                        ++count;
                        if (count == n) {
                            seq_t res = (seq_t)i;
                            free(comp);
                            return res;
                        }
                    }
                }
                /* Mark multiples of i */
                if ((idx_t)i * (idx_t)i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        comp[j] = true;
                }
            }
        }
        free(comp);
        /* Not enough terms – enlarge limit and try again */
        limit *= 2;
    }
}
