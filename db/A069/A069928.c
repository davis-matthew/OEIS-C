/*
 * A069928  Number of integers k, 1<=k<=n, such that tau(k) divides sigma(k)
 *          where tau(k) is the number of divisors of k and sigma(k) the sum
 *          of divisors of k.
 * https://oeis.org/A069928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Card(k: 1<=k<=n : sigma(k) == 0 (mod tau(k)))
 * Range:     u8 n <= 360, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for cumulative counts (index 0 unused) */
static seq_t *cum = NULL;
static idx_t  max_cached = 0;

seq_t a(idx_t n)
{
    /* defined for n>=1; return 0 for n==0 */
    if (n == 0) return 0;

    /* extend cache if necessary */
    if (n > max_cached) {
        /* grow array to size n+1 (indices 0..n) */
        seq_t *new_cum = (seq_t*)realloc(cum, (n + 1) * sizeof(seq_t));
        if (!new_cum) return 0;    /* allocation failed → return 0 */
        cum = new_cum;

        /* initial value for index 0 */
        if (max_cached == 0) cum[0] = 0;

        seq_t prev = cum[max_cached];
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            /* factor k: compute tau(k) and sigma(k) */
            uint64_t tau = 1;
            uint64_t sigma = 1;
            idx_t temp = k;
            for (idx_t p = 2; p <= temp / p; ++p) {
                if (temp % p == 0) {
                    uint64_t exp = 0;
                    uint64_t p_pow = 1;
                    uint64_t sum = 1;
                    while (temp % p == 0) {
                        temp /= p;
                        exp++;
                        p_pow *= (uint64_t)p;
                        sum += p_pow;
                    }
                    tau *= (exp + 1);
                    sigma *= sum;
                }
            }
            if (temp > 1) {                  /* remaining prime factor */
                tau *= 2;
                sigma *= (uint64_t)(1 + temp);
            }

            if (sigma % tau == 0)
                prev += (seq_t)1;
            cum[k] = prev;   /* store cumulative count as seq_t */
        }
        max_cached = n;
    }
    return cum[n];
}
