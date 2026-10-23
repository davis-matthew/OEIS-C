/*
 * A111322  Numbers k such that 8 divides prime(1) + ... + prime(k).
 * https://oeis.org/A111322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that 8 divides (prime(1) + … +
 *            prime(k))
 * Range:     u8 n <= 44, u16 n <= 8229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    /* static state across calls */
    static uint64_t last_prime = 0;    /* the last prime found (0 before any primes) */
    static idx_t    prime_cnt  = 0;    /* number of primes processed so far */
    static int      sum_mod8   = 0;    /* running sum of primes modulo 8 */
    static seq_t   *valid_k    = NULL; /* array of k values with sum%8==0 */
    static idx_t    valid_cap  = 0;    /* capacity of valid_k */
    static idx_t    valid_sz   = 0;    /* number of elements stored */

    /* generate further terms until we have at least n */
    while (valid_sz < n) {
        /* find next prime after last_prime */
        uint64_t cand;
        if (last_prime < 2)
            cand = 2;           /* start at the first prime */
        else {
            cand = last_prime + 1;
            if (cand != 2 && (cand & 1) == 0)
                ++cand;        /* force cand to be odd unless it is 2 */
        }

        while (!is_prime(cand))
            cand += (cand == 2 ? 1 : 2);   /* skip even numbers after 2 */

        /* update state for this new prime */
        ++prime_cnt;
        sum_mod8 = (sum_mod8 + (int)(cand & 7)) & 7; /* sum modulo 8 */

        if (sum_mod8 == 0) {
            /* store prime counter as a valid k */
            push_seq(&valid_k, &valid_cap, &valid_sz, (seq_t)prime_cnt);
        }

        last_prime = cand;
    }

    return valid_k[n - 1];
}
