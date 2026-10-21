/*
 * A079143  Numbers divisible by prime ceilings of their square roots + 1.
 * https://oeis.org/A079143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(ceil(n/2)) * (prime(ceil(n/2)) - (n mod 2))
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined by OEIS, return 0 safely */
        return 0;

    /* k = ceil(n/2) = (n+1)/2 */
    idx_t k = (n + 1) >> 1;

    /* static cache of primes generated on demand */
    static uint64_t *prime_arr   = NULL; /* array of found primes   */
    static size_t   prime_cnt  = 0;      /* number of primes stored */
    static size_t   prime_cap  = 0;      /* capacity of the array   */
    static uint64_t next_candidate = 2;  /* next number to test     */

    /* grow the prime cache until we have k primes */
    while (prime_cnt < k) {
        uint64_t p = next_candidate;
        bool is_prime = true;

        /* trial division by primes already known */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pr = prime_arr[i];
            if (pr * pr > p)
                break;
            if (p % pr == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* make room if necessary */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                uint64_t *tmp =
                    (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
                if (!tmp)          /* allocation failed – give up */
                    return 0;
                prime_arr = tmp;
                prime_cap = new_cap;
            }
            prime_arr[prime_cnt++] = p;
        }

        /* next candidate: skip even numbers after 2 */
        next_candidate = (p == 2) ? 3 : p + 2;
    }

    seq_t prime = (seq_t)prime_arr[k - 1];
    seq_t rem   = (seq_t)(n & 1);   /* n mod 2 */
    return prime * (prime - rem);
}
