/*
 * A082244  Smallest odd prime that is the sum of 2n+1 consecutive primes.
 * https://oeis.org/A082244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements OEIS A082244:  for each n compute the smallest odd
    prime that can be expressed as the sum of 2n+1 consecutive primes.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"   /*  bool isPrimeGeneric(seq_t)  */
#include "../utils/build_primes.h"     /*  void build_primes(idx_t limit, idx_t **out, size_t *cnt)  */

/*  Cache of primes and their prefix sums  */
static idx_t       *prime_list   = NULL;      /* array of primes  */
static size_t        prime_count  = 0;         /* number of primes  */
static seq_t        *prime_prefix = NULL;      /* prefix sums :  prime_prefix[i] = sum of first i primes  */

/*  Ensure at least 'need' primes are available  */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;                               /* already enough   */

    /*  Start with a reasonable limit and increase until we have enough primes  */
    idx_t limit = prime_count ? prime_count * 2 : 1000000;   /* 1e6 primes ~ 15 million  */

    idx_t *tmp_list = NULL;
    size_t tmp_cnt = 0;

    while (1) {
        build_primes(limit, &tmp_list, &tmp_cnt);
        if (tmp_cnt >= need) {
            /*  Replace the old data  */
            if (prime_list) free(prime_list);
            if (prime_prefix) free(prime_prefix);

            prime_list  = tmp_list;
            prime_count = tmp_cnt;

            prime_prefix = (seq_t *)malloc((prime_count + 1) * sizeof(seq_t));
            if (!prime_prefix) {
                /*  In a real program we would handle out‑of‑memory here  */
                abort();
            }
            prime_prefix[0] = 0;
            for (size_t i = 0; i < prime_count; ++i)
                prime_prefix[i + 1] = prime_prefix[i] + (seq_t)prime_list[i];

            return;
        }

        /*  Not enough yet – free and try a larger limit  */
        free(tmp_list);
        limit *= 2;
    }
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    /*  Window length is odd: 2n+1  */
    idx_t L = (idx_t)(2 * n + 1);

    /*  Scan all consecutive windows of length L until an odd prime is found  */
    for (idx_t start = 0; ; ++start) {
        /*  Ensure we have primes for this window  */
        ensure_primes((size_t)(start + L));

        /*  Compute window sum via prefix sums  */
        seq_t sum = prime_prefix[(size_t)(start + L)] - prime_prefix[(size_t)start];

        /*  Check if sum is an odd prime  */
        if ((sum & 1) && isPrimeGeneric(sum))
            return sum;
    }
}
