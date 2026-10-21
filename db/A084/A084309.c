/*
 * A084309  a(n) = gcd(prime(n)-1, n).
 * https://oeis.org/A084309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(prime(n)-1, n) (offset 1)
 * Range:     u8 n <= 54, u16 n <= 8126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* GCD for unsigned integer type seq_t */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- prime generation --------------------------------------- */
static idx_t *prime_list   = NULL;   /* cache of primes (1‑based)          */
static size_t prime_count  = 0;
static size_t prime_cap    = 0;
static idx_t  prime_limit  = 0;      /* maximum number handled by sieve   */
static bool  *sieve        = NULL;   /* current sieve array               */

/* Build a sieve up to 'limit', store primes in prime_list, and
   update prime_count/prime_limit.  The function is called only
   when a higher nth prime is required. */
static void ensure_primes_up_to(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;                       /* already have enough primes      */

    /* --- determine new sieve limit -------------------------------- */
    idx_t new_limit;
    if (n < 6)                       /* small n: use a fixed small limit */
        new_limit = 15;
    else {
        double dn = (double)n;
        new_limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 10);
    }
    if (new_limit <= prime_limit)    /* extend marginally if needed      */
        new_limit = prime_limit + 1000U;

    /* --- allocate and build sieve --------------------------------- */
    bool *new_sieve = (bool *)calloc((size_t)new_limit + 1, sizeof(bool));
    if (!new_sieve)                 /* allocation failed – give up      */
        return;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (!new_sieve[i])
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_sieve[j] = true;
    }

    /* --- collect primes -------------------------------------------- */
    size_t new_count = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!new_sieve[i])
            ++new_count;

    if (prime_cap < new_count) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 128;
        while (new_cap < new_count)
            new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) {                /* allocation failed – clean up      */
            free(new_sieve);
            return;
        }
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* --- store primes ---------------------------------------------- */
    prime_count = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!new_sieve[i])
            prime_list[prime_count++] = i;
    prime_limit = new_limit;

    /* --- replace old sieve ----------------------------------------- */
    free(sieve);
    sieve = new_sieve;
}

/* Return the nth prime, 1‑based.  Calls ensure_primes_up_to as needed. */
static inline idx_t nth_prime(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_primes_up_to(n);
    return prime_list[n - 1];
}

/* ---------- sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* safety for out‑of‑range calls   */

    idx_t p = nth_prime(n);
    seq_t p_minus1 = (seq_t)(p - 1);
    seq_t n_val    = (seq_t)n;
    return gcd_seq_t(p_minus1, n_val);
}
