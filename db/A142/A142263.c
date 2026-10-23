/*
 * A142263  Primes congruent to 14 mod 43.
 * https://oeis.org/A142263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 14 (mod 43) (computed by
 *            searching)
 * Range:     u8 n <= 1, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */
    idx_t limit = 500;              /* initial search bound */

    while (1) {
        /* Allocate boolean array for the sieve */
        char *comp = (char *)calloc((size_t)(limit + 1), 1);
        if (!comp) return 0;        /* allocation failure */

        /* Eratosthenes sieve */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (comp[p]) continue;
            for (idx_t m = p * p; m <= limit; m += p) {
                comp[m] = 1;
            }
        }

        /* Count primes ≡ 14 mod 43 */
        idx_t cnt = 0;
        for (idx_t num = 14; num <= limit; ++num) {
            if (!comp[num] && (num % 43) == 14) {
                ++cnt;
                if (cnt == n) {
                    seq_t result = (seq_t)num;
                    free(comp);
                    return result;
                }
            }
        }

        free(comp);
        /* Increase the bound for the next trial */
        limit = limit * 2 + 1000;
    }
}
