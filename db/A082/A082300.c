/*
 * A082300  Numbers relatively prime to the sum of their prime factors (with
 *          repetition).
 * https://oeis.org/A082300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m ≥ 1 such that gcd(m, sopfr(m)) = 1
 *            Computed by brute‑force search
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    /* Build a small list of primes once.  The limit is generous enough
       for the intended range of n (≈10000). */
    if (primes == NULL) {
        build_primes_upto((idx_t)200000, &primes, &primes_cnt);
    }

    if (n == 1) {
        return (seq_t)1;
    }

    idx_t found = 0;
    seq_t num = 0;

    while (found < n) {
        ++num;                      /* candidate integer m */
        uint64_t val = (uint64_t)num;
        uint64_t temp = val;
        uint64_t sum  = 0;          /* sopfr(m) */

        /* Factorise using the pre‑computed primes */
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
            uint64_t p = primes[i];
            while (temp % p == 0) {
                sum  += p;
                temp /= p;
            }
        }
        if (temp > 1) {
            sum += temp;             /* remaining prime factor */
        }

        if (gcd(num, (seq_t)sum) == (seq_t)1) {
            ++found;                 /* found a new term */
        }
    }

    return num;
}
