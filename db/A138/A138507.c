/*
 * A138507  Expansion of (f(q)^5 / f(q^5) - 1) / 5 in powers of q where f() is
 *          a Ramanujan theta function.
 * https://oeis.org/A138507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 1698, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A138507
    Expansion of (f(q)^5 / f(q^5) - 1) / 5 as a Dirichlet-multiplicative sequence.
    For a prime power p^e:

        - p = 2 :  a(2^e) = ((-2)^(e+1) - 1) / 3
        - p = 5 :  a(5^e) = 1          (b = 0 in the general formula)
        - otherwise (p odd, p ≠ 5):
            let s =  1  if p ≡ 1 or 9 (mod 10),
                     -1 if p ≡ 3 or 7 (mod 10)
            then a(p^e) = ((s·p)^(e+1) - 1) / (s·p - 1)
    The function implements the above by prime factorisation.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* local prime table – built on first use, up to 100 000 (covers n ≤ 10¹⁰) */
static uint32_t primes[9600];          /*≈π(100 000) */
static size_t  prime_cnt   = 0;
static bool    primes_inited = false;
static void init_primes(void)
{
    if (primes_inited) return;
    const int LIMIT = 100000;
    bool sieve[LIMIT + 1];
    memset(sieve, 0, sizeof(sieve));
    for (int i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((1LL * i * i) <= LIMIT)
                for (int j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 1;
        }
    }
    primes_inited = true;
}

/* multiplicative function a(n) */
seq_t a(idx_t n)
{
    init_primes();                    /* prime list ready */

    seq_t result = (seq_t)1;          /* multiplicative accumulation */
    idx_t   m     = n;

    while (m > 1) {
        /* find the smallest prime factor */
        uint32_t p = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t prime = primes[i];
            if ((uint64_t)prime * (uint64_t)prime > m) break;
            if (m % prime == 0) {
                p = prime;
                break;
            }
        }
        if (p == 0) {                    /* m itself is prime > sqrt(original m) */
            p = (uint32_t)m;
        }
        /* exponent e of this prime */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        seq_t a_prime;                  /* contribution of this prime power */
        if (p == 2) {
            /* a(2^e) = ((-2)^(e+1) - 1) / 3 */
            seq_t pow_neg2 = (seq_t)1;
            seq_t base = -(seq_t)2;
            for (idx_t i = 0; i < e + 1; ++i)
                pow_neg2 *= base;
            a_prime = (pow_neg2 - (seq_t)1) / (seq_t)3;
        }
        else if (p == 5) {
            /* a(5^e) = 1 for all e ≥ 1 (b = 0 gives 1) */
            a_prime = (seq_t)1;
        }
        else { /* odd primes ≠ 5 */
            /* determine the sign according to p mod 10 */
            int sign = ((p % 10 == 1) || (p % 10 == 9)) ? 1 : -1;
            seq_t b = (sign == 1) ? (seq_t)p : -(seq_t)p;

            seq_t b_pow = (seq_t)1;
            for (idx_t i = 0; i < e + 1; ++i)
                b_pow *= b;

            seq_t denom = b - (seq_t)1;
            seq_t numer = b_pow - (seq_t)1;

            a_prime = numer / denom;  /* exact division */
        }

        result *= a_prime;           /* accumulate multiplicatively */
    }

    return result;
}
