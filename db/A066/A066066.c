/*
 * A066066  a(n) = prime(2*n) - 2*prime(n).
 * https://oeis.org/A066066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(2*n) - 2*prime(n) (offset 1)
 * Range:     i8 n <= 81, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal sieve based implementation, compatible with any seq_t / idx_t. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/*  Natural‑number types supplied by the build:
        idx_t   – unsigned 64‑bit index type
        seq_t   – signed integer type (int8_t…__int128 or big integer)
   ------------------------------------------------------------ */

/* ---------------------- Prime sieve ------------------------ */

static idx_t           *prime_list   = NULL;   /* 0‑based list of primes   */
static size_t          prime_count   = 0;      /* number of primes stored */
static uint8_t         *sieve       = NULL;   /* temporary boolean array */
static size_t          sieve_limit  = 0;      /* current sieve limit      */
static idx_t           current_needed=0;      /* maximum index requested */

/* Rough upper bound for the m‑th prime when m < 6: 15.
   For larger m an empirical bound m*(20) is sufficient for the given
   limits (n up to about 10000 ⇒ need ≤20000 primes). */
static size_t estimate_limit(idx_t needed)
{
    if (needed < 6)
        return 15;
    /* Use a generous linear bound – enough for the required range. */
    size_t lim = (size_t)needed * 20;
    if (lim < 20)
        lim = 20;
    return lim;
}

/* Build a new sieve up to 'limit' and populate prime_list. */
static void build_sieve(idx_t needed)
{
    size_t limit = estimate_limit(needed);
    /* If we already have a large enough sieve, reuse it. */
    if (sieve && sieve_limit >= limit) {
        /* Ensure we have at least 'needed' primes. */
        if (prime_count >= needed)
            return;
        /* Otherwise extend by re‑sieving larger limit. */
    }

    /* Free previous data if any. */
    free(sieve);
    free(prime_list);
    sieve = (uint8_t *)malloc(limit + 1);
    if (!sieve) exit(1);
    memset(sieve, 0, limit + 1);

    /* Classic Eratosthenes sieve. */
    for (size_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;

    /* Count primes and store them. */
    prime_count = 0;
    /* Over‑allocate to avoid many reallocs. */
    size_t capacity = needed * 2 + 100;
    prime_list = (idx_t *)malloc(sizeof(idx_t) * capacity);
    if (!prime_list) exit(1);

    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i]) {
            if (prime_count >= capacity) {
                capacity *= 2;
                prime_list = (idx_t *)realloc(prime_list, sizeof(idx_t) * capacity);
                if (!prime_list) exit(1);
            }
            prime_list[prime_count++] = (idx_t)i;
            if (prime_count >= needed)
                break;
        }

    sieve_limit = limit;
    current_needed = needed;
}

/* Ensure that at least the first 'needed' primes are available. */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= needed)
        return;
    build_sieve(needed);
}

/* ------------------------ Sequence ------------------------ */

seq_t
a(idx_t n)
{
    /* n ≥ 1 (offset 1). 2*n‑th prime needed. */
    if (n == 0)
        return 0;        /* undefined, guard for safety */

    idx_t need = 2 * n;          /* number of primes required */
    if (need <= 0)  /* overflow guard, though unlikely for given limits */
        need = current_needed > 0 ? current_needed : 1;

    ensure_primes(need);

    seq_t p2n = (seq_t)prime_list[need - 1]; /* 2*n‑th prime (0‑based index) */
    seq_t pn  = (seq_t)prime_list[n - 1];    /* n‑th prime               */

    return p2n - 2 * pn;
}
