/*
 * A172036  Prime partial sums of safe primes A005385.
 * https://oeis.org/A172036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th prime partial sum of safe primes (partial sums that
 *            are themselves prime)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 8, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static state to allow out-of-order calls */
    static idx_t curr = 3;          /* next odd number to test for primality      */
    static seq_t running_sum = 0;   /* running sum of safe primes                */
    static seq_t *partial_sums = NULL;
    static size_t ps_len = 0, ps_cap = 0;

    if (n == 0) return 0; /* offset irrelevant, sequence is 1‑based */

    /* generate terms until we have at least n of them */
    while (ps_len < (size_t)n) {
        /* find next prime */
        while (!is_prime(curr)) {
            curr += 2;                /* skip even numbers */
        }

        /* check if the prime is a safe prime (p-1)/2 is also prime */
        if (is_prime((curr - 1) / 2)) {
            running_sum += (seq_t)curr;  /* add to running sum */

            /* if the sum is prime, store it */
            if (is_prime_big(running_sum)) {
                if (ps_len == ps_cap) {
                    size_t new_cap = ps_cap ? ps_cap * 2 : 4;
                    seq_t *tmp = seq_realloc(partial_sums, ps_cap, new_cap);
                    if (!tmp) return 0; /* allocation failure */
                    partial_sums = tmp;
                    ps_cap = new_cap;
                }
                partial_sums[ps_len++] = running_sum;
            }
        }

        curr += 2;  /* next odd candidate */
    }

    return partial_sums[n - 1];
}
