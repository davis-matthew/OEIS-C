/*
 * A125257  Smallest prime divisor of 4n^2+3 that is of the form 6k+1.
 * https://oeis.org/A125257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A125257:
   the smallest prime divisor of 4n^2+3 that is congruent to 1 modulo 6
   (the first such divisor is returned for each n ≥ 1).  The routine
   builds a short list of primes once and then factorises 4n^2+3 by trial
   division, fully stripping all small prime factors.  After the loop the
   remaining co‑factor is either 1 or a single prime; if it is ≡1 (mod 6)
   it is the desired value.  The algorithm runs in O(π(√(4n^2+3))) time
   per call and works for any seq_t that can store the magnitude of the
   result. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static int primes_initialized = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    /* 20005 > sqrt(4·10000²+3) ≈ 20000, sufficient for all required n. */
    build_primes_upto(20005, &primes, &prime_cnt);
    primes_initialized = 1;
}

seq_t a(idx_t n)
{
    init_primes();

    /* m = 4n² + 3 */
    seq_t nn = (seq_t)n;
    seq_t m  = ((seq_t)4 * nn * nn) + ((seq_t)3);

    seq_t rem = m;           /* remaining factor to be factorised */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t pp = p * p;
        if (pp > rem) break;   /* p exceeds sqrt(rem) */

        if (rem % p == 0)
        {
            /* p is a prime divisor of rem */
            if (p % 6 == 1) return p;   /* first 1 mod 6 divisor found */

            /* remove all powers of p from rem */
            while (rem % p == 0)
                rem /= p;
        }
    }

    /* After removing all small prime factors, rem is either 1 or a single prime.
       If it is prime and ≡1 (mod 6), it is the desired divisor. */
    if (rem > 1 && (rem % 6 == 1))
        return rem;

    /* By the definition of the sequence, a divisor always exists; the
       following return value is only defensive. */
    return (seq_t)0;
}
