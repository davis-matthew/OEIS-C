/*
 * A022461  a(n) = prime(n+1)*prime(n+2) mod prime(n).
 * https://oeis.org/A022461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1) * prime(n+2) mod prime(n)
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 8693, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_upper, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_prime_upper.h"

/* Cache of primes: primes[0] = 2 = prime(1) */
static idx_t *primes = NULL;
static size_t  primes_len = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1; guard against 0 */

    size_t need = (size_t)(n + 2);   /* need primes up to prime(n+2) */
    if (primes_len < need) {
        idx_t limit = _prime_upper(need);
        while (1) {
            uint64_t *tmp = NULL;
            size_t cnt = 0;
            build_primes_upto(limit, &tmp, &cnt);
            if (cnt < need) {
                free(tmp);
                limit *= 2;            /* increase bound and try again */
                continue;
            }
            free(primes);              /* free previous cache */
            primes = (idx_t *)tmp;
            primes_len = cnt;
            break;
        }
    }

    /* primes are 0‑indexed: prime(1) == primes[0] */
    seq_t p_n  = (seq_t)primes[(size_t)(n - 1)];
    seq_t p_np1 = (seq_t)primes[(size_t)n];     /* prime(n+1) */
    seq_t p_np2 = (seq_t)primes[(size_t)(n + 1)]; /* prime(n+2) */

    seq_t prod = p_np1 * p_np2;   /* product fits within seq_t for the tested range */
    return prod % p_n;
}
