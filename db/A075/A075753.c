/*
 * A075753  Smallest prime factor of n-th odd triangular number; a(1) = 1.
 * https://oeis.org/A075753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of the n‑th odd triangular number
 *            (a(1) = 1). The odd triangular numbers are T_m for m ≡ 1 or 2
 *            (mod 4)
 * Range:     u8 n <= 12, u16 n <= 2078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------- */
/*  Prime sieve – used to factor triangular numbers efficiently.       */
/* ------------------------------------------------------------------- */

#define PRIME_SIEVE_LIMIT 20000            /* > sqrt(2·10⁴²) ≈ 14142      */
static int  primes[PRIME_SIEVE_LIMIT];
static int  prime_cnt = 0;
static bool primes_ready = false;
static bool is_composite[PRIME_SIEVE_LIMIT + 1];   /* zero‑initialized globally */

/* Build a simple sieve of primes up to PRIME_SIEVE_LIMIT. */
static void init_primes(void)
{
    if (primes_ready) return;

    is_composite[0] = is_composite[1] = true;
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((int64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* ------------------------------------------------------------------- */
/*  Smallest prime factor – works for 1 ≤ n ≤ UINT64_MAX (trivial case  */
/*  for n == 1) and uses the pre‑computed prime list.                  */
/* ------------------------------------------------------------------- */

static uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n <= 1) return n;               /* 1 stays 1, 0 or negative not used */
    if (n % 2 == 0) return 2;
    for (int i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > n) break;
        if (n % p == 0) return p;
    }
    return n;                            /* n itself is prime */
}

/* ------------------------------------------------------------------- */
/*  Main sequence function:  a(idx_t n)                               */
/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Safety – the sequence is 1‑indexed.  If someone passes 0 we just */
    /* return 0 to avoid undefined behaviour.                         */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    init_primes();

    /* Determine the index m of the n‑th odd triangular number:
       m ≡ 1 (mod 4) for odd n, and m ≡ 2 (mod 4) for even n. */
    idx_t k = (n - 1) / 2;                /* floor((n-1)/2) */
    idx_t m = 4 * k + (n & 1 ? 1 : 2);

    /* Triangular number T_m = m·(m+1)/2  (fits comfortably in uint64). */
    seq_t T = (seq_t)m * ((seq_t)m + 1) / 2;

    if (T == 1) return (seq_t)1;          /* only for n = 1, already handled */

    /* Find the smallest prime factor of T.  The value of T for n ≤ 10⁴   */
    /* is less than 2·10⁸, so 64‑bit arithmetic is sufficient.       */
    uint64_t lpf = smallest_prime_factor_uint64((uint64_t)T);

    return (seq_t)lpf;
}
