/*
 * A056068  Numbers k such that k! is divisible by the square of (f+d)!^2 for
 *          d = 0, 1 and 2 (and possibly larger d), where f = floor(k/2).
 * https://oeis.org/A056068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that k! is divisible by
 *            ((floor(k/2)+2)!)^2
 * Range:     u8 none, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Maximum limit for prime sieve; sufficient for n up to about 10 000. */
#define SIEVE_LIMIT 5000000

/* Global static data ---------------------------------------------------- */
static int initialized = 0;          /* initialization flag              */
static uint32_t *spf = NULL;          /* smallest prime factor array      */
static int *prime_index = NULL;       /* index of a prime, -1 otherwise   */
static uint32_t *primes = NULL;       /* list of primes                   */
static int primes_count = 0;          /* number of primes                 */
static int *delta = NULL;             /* exp_k[p] - 2*exp_t[p] for primes*/
static int bad_count = 0;             /* number of primes with negative delta */

static seq_t *seq_values = NULL;     /* sequence values                  */
static size_t seq_cap = 0;
static size_t seq_len = 0;

static uint64_t last_k = 0;          /* last processed k                 */
static uint32_t current_t = 0;        /* current t (=floor(k/2)+2)        */

/* --------------------------------------------------------------------- */
/*  Helper to factorize a number and update delta with given multiplier  */
static void factor_and_update(uint32_t n, int multiplier)
{
    while (n > 1u) {
        uint32_t p = spf[n];
        int cnt = 0;
        while (n % p == 0u) {
            n /= p;
            cnt++;
        }
        int idx = prime_index[p];
        /* idx will always be valid because p is prime */
        int old = delta[idx];
        int newv = old + multiplier * cnt;
        if (old >= 0 && newv < 0) bad_count++;
        else if (old < 0 && newv >= 0) bad_count--;
        delta[idx] = newv;
    }
}

/* --------------------------------------------------------------------- */
/*  Append value to growing sequence array                              */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_values = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_values[seq_len++] = val;
}

/* --------------------------------------------------------------------- */
/*  Initialize sieve and related structures                              */
static void init_sieve(void)
{
    /* allocate arrays */
    spf = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    prime_index = (int *)malloc((SIEVE_LIMIT + 1) * sizeof(int));
    primes = (uint32_t *)malloc(SIEVE_LIMIT * sizeof(uint32_t)); /* enough */
    for (int i = 0; i <= SIEVE_LIMIT; i++) prime_index[i] = -1;

    /* linear sieve */
    int pc = 0;
    for (uint32_t i = 2; i <= (uint32_t)SIEVE_LIMIT; i++) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[pc++] = i;
        }
        for (int j = 0; j < pc && primes[j] <= spf[i] && (uint64_t)i * primes[j] <= (uint32_t)SIEVE_LIMIT; j++) {
            spf[i * primes[j]] = primes[j];
        }
    }
    primes_count = pc;
    for (int i = 0; i < primes_count; i++) {
        prime_index[primes[i]] = i;
    }

    delta = (int *)calloc(primes_count, sizeof(int));

    /* initialize sequence state */
    last_k = 0;
    current_t = 0;
    bad_count = 0;
    seq_values = NULL;
    seq_cap = 0;
    seq_len = 0;
}

/* --------------------------------------------------------------------- */
/*  Build sequence up to desired index n                                 */
static void build_up_to(idx_t n)
{
    while (seq_len < n) {
        last_k++;

        /* update factorial exponents for new k */
        factor_and_update((uint32_t)last_k, +1);

        /* compute new t = floor(k/2) + 2 */
        uint32_t f = (uint32_t)(last_k >> 1);
        uint32_t new_t = f + 2;

        /* update factorial exponents for any t values missed */
        while (current_t < new_t) {
            current_t++;
            factor_and_update(current_t, -2);
        }

        /* if all prime inequalities hold, record k */
        if (bad_count == 0) {
            seq_append((seq_t)last_k);
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = 1;
        init_sieve();
    }
    build_up_to(n);
    return seq_values[n - 1];
}
