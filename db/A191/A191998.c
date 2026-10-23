/*
 * A191998  Numerators of partial products of a Hardy-Littlewood constant.
 * https://oeis.org/A191998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of ∏_{j=1..n} (1 - χ₂(4,p_j)/(p_j-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where χ₂(4,p) =  0 if p=2, +1 if p≡1 (mod 4), –1 if p≡3 (mod 4). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;

static void ensure_primes(idx_t n)
{
    if (n <= prime_cnt) return;
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt  = 0;
    }
    sieve_first_n_primes(n, &prime_list, &prime_cnt);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless   */
    ensure_primes(n);                      /* primes[0 .. n-1] exist    */

    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (size_t i = 0; i < (size_t)n; ++i) {
        uint32_t p = prime_list[i];
        if (p == 2u) continue;            /* factor equals 1             */

        seq_t fnum, fden;
        if ((p & 3u) == 1u) {              /* p ≡ 1 (mod 4)               */
            fnum = (seq_t)(p - 2u);
            fden = (seq_t)(p - 1u);
        } else {                           /* p ≡ 3 (mod 4)               */
            fnum = (seq_t)p;
            fden = (seq_t)(p - 1u);
        }

        num *= fnum;
        den *= fden;

        seq_t g = gcd(num, den);
        if (g != (seq_t)1) {
            num /= g;
            den /= g;
        }
    }
    return num;
}
