/*
 * A144489  Partial sums of A087624.
 * https://oeis.org/A144489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = count of pairs (p,q) with 1 < p < q ≤ n, p prime and p | q. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Simple sieve of Eratosthenes up to n. */
    uint8_t *sieve = (uint8_t *)calloc((size_t)n + 1, sizeof(uint8_t));
    if (!sieve)
        return 0;      /* allocation failure – unlikely for n ≤ 10000 */

    seq_t sum = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (!sieve[i]) {          /* i is prime */
            /* Count multiples of i that are strictly greater than i and ≤ n. */
            idx_t multiples = n / i - 1;   /* floor(n/i) - 1, non‑negative */
            sum += (seq_t)multiples;

            /* Mark all multiples of i as composite. */
            for (idx_t m = i + i; m <= n; m += i)
                sieve[m] = 1;
        }
    }

    free(sieve);
    return sum;
}
