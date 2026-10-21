/*
 * A087882  a(n) = Prime[Abs[n-Prime[n-1]]] + PrimePi[ n- PrimePi[n-2]].
 * https://oeis.org/A087882
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime[Abs[n-Prime[n-1]]] + PrimePi[n-PrimePi[n-2]]
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build */
extern seq_t a(idx_t n);

/* ---------  Local data structures  --------- */
static const idx_t SIEVE_LIMIT = 2000000;   /* large enough for n ≤ 10000  */
static bool *sieve = NULL;                /* sieve[0..LIMIT]  */
static idx_t *primes = NULL;              /* list of primes   */
static idx_t *pi_arr = NULL;              /* pi_arr[i] = π(i) */
static size_t primes_cnt = 0;              /* actual number of primes stored */
static bool initialized = false;

/* ---------  Prime generation  --------- */
static void init_primes(void)
{
    if (initialized)
        return;

    const size_t size = (size_t)(SIEVE_LIMIT + 1);
    sieve = (bool *)calloc(size, sizeof(bool));
    if (!sieve)
        abort();

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (!sieve[p]) {
            for (idx_t q = p * p; q <= SIEVE_LIMIT; q += p)
                sieve[q] = true;
        }
    }

    /* Prime list and π() array */
    size_t cap = 200000;                     /* initial capacity */
    primes = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!primes)
        abort();
    pi_arr  = (idx_t *)calloc(size, sizeof(idx_t));
    if (!pi_arr)
        abort();

    idx_t pi_prev = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            if (primes_cnt == cap) {
                cap <<= 1; /* grow */
                idx_t *tmp = (idx_t *)realloc(primes, cap * sizeof(idx_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            ++pi_prev;
        }
        pi_arr[i] = pi_prev;
    }

    free(sieve);
    initialized = true;
}

/* ---------  Sequence a(n)  --------- */
seq_t a(idx_t n)
{
    /* Build prime data on first call */
    init_primes();

    /* For n < 4 the sequence is undefined – return 0 */
    if (n < 4)
        return (seq_t)0;

    /* -------- 1st term: Prime[ |n - Prime[n-1]| ]  -------- */
    /* Prime[n-1]  (primes array is 0‑based) */
    idx_t prime_n_minus1 = primes[n - 2];
    idx_t diff = (n > prime_n_minus1) ? (n - prime_n_minus1)
                                      : (prime_n_minus1 - n);
    if (diff == 0)          /* should not happen for n ≥ 4 */
        return (seq_t)0;
    idx_t first_prime = primes[diff - 1];   /* k‑th prime, k = diff */

    /* -------- 2nd term: PrimePi[ n - PrimePi[n-2] ]  -------- */
    idx_t pi_sub = pi_arr[n - 2];                     /* PrimePi[n-2] */
    idx_t m = n - pi_sub;                             /* n - π(n-2) */
    idx_t pi_m = pi_arr[m];                           /* π(m) */

    return (seq_t)first_prime + (seq_t)pi_m;
}
