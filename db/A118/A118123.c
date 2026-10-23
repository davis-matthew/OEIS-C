/*
 * A118123  a(n) = number of k's such that prime(n+1) = prime(n) + (prime(n)
 *          mod k).
 * https://oeis.org/A118123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k>0 | prime(n+1) - prime(n) = prime(n) % k }
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Recursively count divisors > diff given the prime factorisation of m */
static void count_divs(const uint64_t *primes, const int *exps, int nf,
                       uint64_t cur, uint64_t diff, uint64_t *cnt)
{
    if (nf == 0) {
        if (cur > diff) (*cnt)++;
        return;
    }
    uint64_t p = primes[0];
    int e = exps[0];
    uint64_t val = cur;
    for (int i = 0; i <= e; ++i) {
        count_divs(primes + 1, exps + 1, nf - 1, val, diff, cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t pn = (uint64_t)get_prime(n);
    uint64_t pn_next = (uint64_t)get_prime(n + 1);
    uint64_t diff = pn_next - pn;                   /* p_{n+1} - p_n   */
    uint64_t m = 2 * pn - pn_next;                  /* 2*p_n - p_{n+1} */
    if (m == 0) return 0;                           /* should not happen */

    /* Factorise m by trial division */
    uint64_t primes_arr[32];
    int exps_arr[32];
    int nf = 0;
    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes_arr[nf] = p;
            exps_arr[nf] = e;
            ++nf;
        }
    }
    if (temp > 1) {                                 /* remaining prime */
        primes_arr[nf] = temp;
        exps_arr[nf] = 1;
        ++nf;
    }

    uint64_t count = 0;
    count_divs(primes_arr, exps_arr, nf, 1, diff, &count);
    return (seq_t)count;
}
