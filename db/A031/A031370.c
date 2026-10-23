/*
 * A031370  a(n) = prime(4n-1).
 * https://oeis.org/A031370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(4n‑1)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes.h"  /* provides void build_primes(idx_t limit, idx_t **out, size_t *cnt); */

static idx_t *prime_arr   = NULL;   /* cached primes (built by build_primes) */
static size_t prime_cnt   = 0;      /* number of primes stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1, safe guard */

    /* target is the (4n‑1)th prime (1‑based index) */
    idx_t target = 4 * n - 1;
    if (target == 0) return 0;        /* should not happen for n>=1 */

    /* Return quickly if already computed */
    if (target <= prime_cnt)
        return (seq_t)prime_arr[target - 1];

    /* Estimate an upper bound for the target‑th prime.  */
    /* For k ≥ 6  :  p_k < k (log k + log log k).                     */
    idx_t limit;
    if (target < 6) {
        limit = 100;                  /* generous upper bound for small k */
    } else {
        double k = (double)target;
        limit = (idx_t)(k * (log(k) + log(log(k)))) + 10;
    }

    /* Grow the prime list until it contains the requested index */
    while (1) {
        if (prime_arr != NULL) {
            free(prime_arr);
            prime_arr = NULL;
            prime_cnt = 0;
        }

        build_primes(limit, &prime_arr, &prime_cnt);

        if (prime_cnt >= target) break;

        /* Not enough primes – enlarge the limit and try again */
        limit <<= 1;
    }

    return (seq_t)prime_arr[target - 1];
}
