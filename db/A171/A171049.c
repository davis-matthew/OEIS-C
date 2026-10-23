/*
 * A171049  In the sequence of prime numbers, replace all the '9' digits with
 *          '0' and vice versa.
 * https://oeis.org/A171049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A171049: replace all 9's with 0's and all 0's with 9's in the
 *  n-th prime number (offset 1).  The function generates primes on demand
 *  and transforms the required prime.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t *prime_vec   = NULL;   /* list of primes (idx_t) */
static size_t prime_cap  = 0;       /* allocated capacity */
static size_t prime_len  = 0;       /* number of primes stored */
static idx_t   next_cand  = 2;      /* next integer to test for primality */

/* transform a number by swapping digits 9 ↔ 0 */
static seq_t
transform(seq_t v)
{
    uint8_t digs[64];   /* enough for 128-bit numbers */
    size_t  dlen = 0;
    seq_t   tmp = v;

    if (tmp == 0)
        return 0;      /* not expected for primes */

    while (tmp > 0) {
        digs[dlen++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t res = 0;
    for (size_t i = 0; i < dlen; ++i) {
        uint8_t d = digs[dlen - 1 - i];
        uint8_t nd = d;
        if (d == 9)        nd = 0;
        else if (d == 0)   nd = 9;
        res = res * 10 + nd;
    }
    return res;
}

/* generate primes until we have at least n primes stored */
static void
ensure_primes(idx_t n)
{
    if (prime_len >= n)
        return;

    while (prime_len < n) {
        idx_t m = next_cand;
        bool isprime = true;

        for (size_t i = 0; i < prime_len; ++i) {
            idx_t p = prime_vec[i];
            if (p * p > m)
                break;
            if (m % p == 0) {
                isprime = false;
                break;
            }
        }

        if (isprime) {
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_vec = (idx_t *)realloc(prime_vec,
                                             prime_cap * sizeof(idx_t));
            }
            prime_vec[prime_len++] = m;
        }
        ++next_cand;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined, but harmless for offset 0 */

    ensure_primes(n);
    seq_t prime = (seq_t)prime_vec[n - 1];
    return transform(prime);
}
