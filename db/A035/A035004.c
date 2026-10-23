/*
 * A035004  Number of divisors of the n-th nonprime.
 * https://oeis.org/A035004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = d(A018252(n)) // number of divisors of the nth nonprime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* Return the number of divisors of an uint64_t value using trial division.
   The result is stored in seq_t to support arbitrary‑precision builds. */
static seq_t divisor_count(idx_t val)
{
    seq_t res = (seq_t)1;
    idx_t n = val;

    /* handle factor 2 separately for efficiency */
    if (n % 2 == 0) {
        int exp = 0;
        do { n /= 2; ++exp; } while (n % 2 == 0);
        res *= (seq_t)(exp + 1);
    }

    /* loop over odd candidates */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            int exp = 0;
            do { n /= p; ++exp; } while (n % p == 0);
            res *= (seq_t)(exp + 1);
        }
    }

    /* if any prime factor remains > 1 */
    if (n > 1) res *= (seq_t)2;

    return res;
}

/* Main sequence function: number of divisors of the n-th nonprime. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – out‑of‑range */

    idx_t count = 0;                      /* how many nonprimes seen so far */

    for (idx_t candidate = 1;; ++candidate) {
        /* primality test via simple trial division */
        bool is_prime;
        if (candidate < 2) {
            is_prime = false;            /* 1 is not prime */
        } else if (candidate == 2) {
            is_prime = true;
        } else if (candidate % 2 == 0) {
            is_prime = false;
        } else {
            is_prime = true;
            for (idx_t p = 3; p <= candidate / p; p += 2) {
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) continue;          /* skip primes */

        ++count;                          /* candidate is a nonprime (composite or 1) */
        if (count == n) return divisor_count(candidate);
    }
}
