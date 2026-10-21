/*
 * A098130  Concatenation of consecutive primes in the forward and reverse
 *          order.
 * https://oeis.org/A098130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   estimate_upper, rev_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concat(p1, p2, …, pn, rev(pn), rev(pn-1), …, rev(p1))
 * where pi is the i‑th prime and rev(x) is the decimal digit reversal of x.
 *
 * The function generates as many primes as needed, then builds the
 * concatenated number using only integer arithmetic.  It works for any
 * of the supported seq_t types (fixed‑width or arbitrary precision).
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/estimate_upper.h"
#include "../utils/rev_uint32.h"

/* ---------- prime generation ---------- */
static bool *sieve_mask  = NULL;   /* sieve flag array   */
static uint32_t *primes  = NULL;   /* list of primes     */
static size_t   prime_cnt = 0;     /* number of primes   */
static size_t   sieve_limit = 0;   /* size of sieve     */

/* perform a sieve up to 'limit' and store all primes found */
static void sieve_primes(size_t limit)
{
    /* free any previous data */
    if (sieve_mask) free(sieve_mask);
    if (primes)     free(primes);

    sieve_mask = (bool*)malloc(limit + 1);
    if (!sieve_mask) return;                 /* allocation failed */
    memset(sieve_mask, true, limit + 1);
    sieve_mask[0] = sieve_mask[1] = false;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (sieve_mask[p]) {
            for (size_t k = p * p; k <= limit; k += p)
                sieve_mask[k] = false;
        }
    }

    /* count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve_mask[i]) ++prime_cnt;

    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) return;                     /* allocation failed */

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve_mask[i]) primes[idx++] = (uint32_t)i;

    sieve_limit = limit;
}

/* ensures that we have at least 'n' primes available */
static void ensure_primes(size_t n)
{
    if (prime_cnt >= n) return;                /* already enough */

    size_t limit = estimate_upper(n);
    if (limit <= sieve_limit) {                /* not enough primes stored */
        while (prime_cnt < n) {
            limit *= 2;                         /* double the limit */
            sieve_primes(limit);
        }
    } else {
        sieve_primes(limit);                    /* compute anew */
    }
}

/* number of decimal digits of a 32‑bit unsigned integer (up to 1 000 000) */
static inline int num_digits(uint32_t x)
{
    if (x < 10)          return 1;
    if (x < 100)         return 2;
    if (x < 1000)        return 3;
    if (x < 10000)       return 4;
    if (x < 100000)      return 5;
    return 6;            /* 6 digits for primes < 1 000 000 */
}

static const uint32_t pow10_int[7] = {1,10,100,1000,10000,100000,1000000};

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* out of range */

    ensure_primes((size_t)n);

    seq_t result = 0;
    /* forward part: primes 1..n */
    for (size_t i = 0; i < (size_t)n; ++i) {
        uint32_t p   = primes[i];
        int d        = num_digits(p);
        result = result * (seq_t)pow10_int[d] + (seq_t)p;
    }

    /* reverse part: primes n..1 with each prime reversed */
    for (int i = (int)n - 1; i >= 0; --i) {
        uint32_t p   = primes[i];
        uint32_t rev = rev_uint32(p);
        int d        = num_digits(p);
        result = result * (seq_t)pow10_int[d] + (seq_t)rev;
    }

    return result;
}
