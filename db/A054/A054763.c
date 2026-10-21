/*
 * A054763  Residues of consecutive prime differences modulo 6.
 * https://oeis.org/A054763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001223(n) mod 6 A001223(n) is the n‑th prime difference,
 *            i.e. p_{n+1} – p_n, with offset 1. For n >= 1 this returns the
 *            difference of the (n+1)‑th and n‑th primes modulo 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static storage for primes that the routine generates on demand. */
static uint32_t *primes_arr   = NULL;   /* array of primes (zero‑based) */
static size_t   prime_count   = 0;      /* number of primes stored   */
static size_t   sieve_limit   = 0;      /* upper bound used in sieve */

/* ------------------------------------------------------------ */
/* Estimate an upper bound for the (n+1)-th prime (0‑based
   index used for n).  Uses the inequality
   p_m < m(ln m + ln ln m) for m >= 6.  The result is rounded
   up and a tiny safety margin is added.                    */
static size_t estimate_limit_for_n(idx_t n)
{
    size_t m = (size_t)(n + 1);          /* we need at least m primes */
    if (m < 6) return 15;               /* p_6 = 13, so 15 is safe   */
    double dm = (double)m;
    double lim = dm * (log(dm) + log(log(dm)));
    return (size_t)(lim) + 3;
}

/* ------------------------------------------------------------ */
/* Generate all primes up to 'limit' (inclusive) and
   store them in the static array.  Any previously stored
   primes are freed.                                            */
static void generate_primes_upto(size_t limit)
{
    uint8_t *is_prime = (uint8_t*)malloc(limit + 1);
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    size_t sq = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sq; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *new_arr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_arr) {
        free(is_prime);
        exit(1);
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_arr[idx++] = (uint32_t)i;

    free(is_prime);
    free(primes_arr);      /* free previous array, if any */
    primes_arr   = new_arr;
    prime_count  = cnt;
    sieve_limit  = limit;
}

/* ------------------------------------------------------------ */
/* Ensure that at least n+1 primes (zero‑based index n) are
   available.  If not, regenerate the sieve with a larger bound. */
static void ensure_primes(idx_t n)
{
    if (prime_count > n) return;            /* already enough primes */
    size_t limit = estimate_limit_for_n(n);
    if (limit <= sieve_limit) return;       /* previous run already covered */
    generate_primes_upto(limit);
}

/* ------------------------------------------------------------ */
/* Main sequence function.  For n == 0, a(0) is undefined, but
   we return 0 to avoid out‑of‑bounds access.  For n >= 1
   returns the prime difference modulo 6. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of range index */
    ensure_primes(n);
    /* primes_arr[0] = 2, primes_arr[1] = 3, ...          */
    uint32_t d = primes_arr[n] - primes_arr[n - 1];
    unsigned int r = d % 6;                 /* 0-5 fits any seq_t */
    return (seq_t)r;
}
