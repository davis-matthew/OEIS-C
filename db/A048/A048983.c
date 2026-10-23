/*
 * A048983  As n runs through composite numbers, a(n) = number of composite d
 *          < n such that gcd(d,n) = 1.
 * https://oeis.org/A048983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of composite d < m (m is n­th composite) such that gcd(d,m)=1
   (computed by a sieve and trial gcd) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* Estimate an upper bound for the n‑th composite */
    idx_t limit = n * 2 + 10;
    bool *is_prime = NULL;
    idx_t comp_cnt = 0;
    idx_t m = 0;

    /* Find a limit wide enough to contain at least n composites */
    while (1) {
        is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime) return 0;          /* allocation failure */

        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (idx_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= limit; j += p)
                    is_prime[j] = false;
            }
        }

        comp_cnt = 0;
        for (idx_t i = 4; i <= limit; ++i) {
            if (!is_prime[i]) {
                ++comp_cnt;
                if (comp_cnt == n) { m = i; break; }
            }
        }

        if (comp_cnt >= n) break;
        free(is_prime);
        limit *= 2;
    }

    seq_t count = 0;
    for (idx_t d = 4; d < m; ++d) {
        if (!is_prime[d]) {
            seq_t g = gcd((seq_t)d, (seq_t)m);
            if (g == 1) ++count;
        }
    }
    free(is_prime);
    return count;
}
