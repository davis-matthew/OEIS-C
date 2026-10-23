/*
 * A022891  n-th prime p(k) such that p(k) + p(k+6) = p(k+2) + p(k+4).
 * https://oeis.org/A022891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p(k) such that p(k)+p(k+6) =
 *            p(k+2)+p(k+4). Computed by searching primes
 * Range:     u8 n <= 5, u16 n <= 447, u32 n <= 501, u64 n <= 501,
 *            u128 n <= 501, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_tbl = NULL;
static size_t   prime_cnt = 0;
static idx_t    prime_lim = 0;

/* Ensure that prime_tbl contains at least 'up_to_index' (0‑based). */
static void ensure_primes(idx_t up_to_index)
{
    while (prime_cnt <= (size_t)up_to_index) {
        idx_t new_lim = prime_lim ? prime_lim * 2 : 1000;   /* grow factor 2 */
        uint64_t *tmp   = NULL;
        size_t    tmp_c = 0;

        /* primes up to new_lim, inclusive */
        build_primes_upto(new_lim, &tmp, &tmp_c);

        /* replace old table */
        free(prime_tbl);
        prime_tbl = tmp;
        prime_cnt = tmp_c;
        prime_lim = new_lim;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but keeps implementation safe */

    idx_t  k = 3;                  /* k is 1‑based index of p(k) */
    size_t found = 0;

    for (;;) {
        /* We need primes up to p(k+6) → index k+6-1 */
        ensure_primes(k + 6 - 1);

        seq_t pk  = (seq_t)prime_tbl[k - 1];
        seq_t pk2 = (seq_t)prime_tbl[(k + 2) - 1];
        seq_t pk4 = (seq_t)prime_tbl[(k + 4) - 1];
        seq_t pk6 = (seq_t)prime_tbl[(k + 6) - 1];

        if (pk + pk6 == pk2 + pk4) {
            ++found;
            if (found == n) return pk;
        }
        ++k;
    }
}
