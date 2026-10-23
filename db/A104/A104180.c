/*
 * A104180  Let f[n]=Prime[n+1]-Prime[n]; a(n) = Binomial[Prime[12],f[n]].
 * https://oeis.org/A104180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Binomial[Prime[12], Prime[n+1] - Prime[n]] where
 *            Prime[12] = 37. The binomial is taken with 37 as the upper
 *            argument and the difference between consecutive primes as the
 *            lower argument. If the difference exceeds 37, the result is zero
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- local definitions ---- */
#define PRIME12 37
#define MAX_PRIME_INDEX 20000   /* enough for up to 10,000 requested terms   */
#define SIEVE_LIMIT 200000      /* primes up to this value are generated */

/* static state */
static bool init_done = false;
static idx_t primes[MAX_PRIME_INDEX];        /* stores the first primes[0] = 2, ...   */
static size_t prime_count = 0;
static seq_t binom[PRIME12 + 1];             /* binom[k] = C(37, k) for 0 <= k <= 37 */

static void init_sequence(void)
{
    if (init_done)
        return;

    /* ---- sieve of Eratosthenes up to SIEVE_LIMIT ---- */
    static bool is_prime[SIEVE_LIMIT + 1];
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    idx_t limit = (idx_t)(SIEVE_LIMIT / SIEVE_LIMIT); /* root for clarity */
    for (idx_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= SIEVE_LIMIT; mult += p)
                is_prime[mult] = false;
        }
    }

    /* ---- collect primes into array ---- */
    prime_count = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT && prime_count < MAX_PRIME_INDEX; ++i) {
        if (is_prime[i]) {
            primes[prime_count++] = i;
        }
    }

    /* ---- pre‑compute binomial coefficients C(37, k) ---- */
    binom[0] = (seq_t)1;
    for (idx_t k = 1; k <= PRIME12; ++k) {
        /* C(37,k) = C(37,k-1) * (37 - k + 1) / k */
        seq_t num = binom[k - 1] * (seq_t)(PRIME12 - k + 1);
        seq_t denom = (seq_t)k;
        binom[k] = num / denom;   /* exact division */
    }

    init_done = true;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset is 1 */

    init_sequence();

    /* ensure we have enough primes ; array size is sufficient for n <= 10000 */
    if (n >= prime_count)
        return (seq_t)0;   /* safety check, though should not happen */

    /* difference between Prime[n+1] and Prime[n] */
    seq_t diff = (seq_t)(primes[n] - primes[n - 1]); /* primes[0]=2 => f[1] uses primes[1]-primes[0] */

    if ((idx_t)diff <= PRIME12)
        return binom[(idx_t)diff];
    else
        return (seq_t)0;
}
