/*
 * A179398  a(n) = prime(n)^2 mod prime(n-3).
 * https://oeis.org/A179398
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 mod prime(n-3) (offset 4)
 * Range:     u8 n <= 54, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   _prime_limit_for_idx, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* We use the utilities provided in the build */
#include "../utils/sieve_primes.h"
#include "../utils/_prime_limit_for_idx.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4. For n < 4 we return 0. */
    if (n < 4) return 0;

    /* We keep a static list of primes so that multiple calls do not
       recompute the sieve. */
    static idx_t *prime_table = NULL;   /* primes[0] = 2 */
    static size_t prime_cnt  = 0;
    static idx_t  prime_limit = 0;      /* maximum prime computed so far */

    /* Need primes up to index n.  Use the helper that gives an upper
       bound on the nth prime. */
    idx_t need_limit = _prime_limit_for_idx(n + 1);

    if (prime_cnt < (size_t)(n + 1) || need_limit > prime_limit) {
        /* Recompute the sieve with a larger limit. */
        if (prime_table) free(prime_table);
        sieve_primes(need_limit, &prime_table, &prime_cnt);
        prime_limit = need_limit;
    }

    /* Get the required primes.  prime_table is 0‑based:
         prime_table[0] = 2 = prime(1) */
    idx_t p_n        = prime_table[n - 1];  /* prime(n)   */
    idx_t p_n_minus3 = prime_table[n - 4];  /* prime(n-3) */

    /* Compute prime(n)^2 mod prime(n-3).  We cast to seq_t to
       accommodate whatever width seq_t may have. */
    seq_t pn = (seq_t)p_n;
    seq_t res = (pn * pn) % (seq_t)p_n_minus3;
    return res;
}
