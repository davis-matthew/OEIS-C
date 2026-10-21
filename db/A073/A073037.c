/*
 * A073037  Take A000040, omit commas: 23571113171923..., select 4-digit
 *          primes seen when scanning from left.
 * https://oeis.org/A073037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth 4‑digit prime that occurs in the left‑to‑right
   sliding window over the concatenated prime sequence 2,3,5,7,11,13,17,… */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*   Utility: a dynamic list of primes used for trial division and
    to test the 4‑digit window.                                      */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;  /* primes found so far   */
static size_t   prime_len    = 0;      /* number of primes      */
static size_t   prime_cap    = 0;      /* allocated capacity    */

/* Expand the prime list when required. */
static void grow_prime_list(void)
{
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    uint64_t *tmp   = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();      /* allocation failure */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Append a new prime to the list. */
static void push_prime(uint64_t p)
{
    if (prime_len + 1 > prime_cap) grow_prime_list();
    prime_list[prime_len++] = p;
}

/* ------------------------------------------------------------------ */
/*   Pre‑compute all primes ≤ 100 000 to speed up the primality test
    of the generated primes.                                         */
/* ------------------------------------------------------------------ */
static bool init_small_primes(void)
{
    const uint32_t LIMIT = 100000;   /* > sqrt(10^9) – enough for the problem */
    static bool done = false;
    if (done) return true;

    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve) abort();          /* allocation failure */

    for (uint32_t i = 2; i <= LIMIT; ++i) sieve[i] = true;
    for (uint32_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;

    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) push_prime(i);

    free(sieve);
    done = true;
    return true;
}

/* ------------------------------------------------------------------ */
/*   Test whether n is a prime number.  Uses trial division by all
    stored primes up to sqrt(n).                                    */
/* ------------------------------------------------------------------ */
static bool isPrimeLarge(uint64_t n)
{
    if (n < 2) return false;
    if (!prime_len) init_small_primes();  /* guarantees prime_list is filled */

    for (size_t i = 0; i < prime_len; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > n / p) break;   /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*   Store the discovered 4‑digit primes in order of appearance.    */
/* ------------------------------------------------------------------ */
static uint64_t *results = NULL;  /* array of 4‑digit primes (fits in 16‑bit) */
static size_t   res_len  = 0;     /* number of primed stored */
static size_t   res_cap  = 0;     /* allocated capacity              */

static void grow_results(void)
{
    size_t new_cap = res_cap ? res_cap << 1 : 32;
    uint64_t *tmp   = (uint64_t *)realloc(results, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    results = tmp;
    res_cap = new_cap;
}

static void push_result(uint64_t v)
{
    if (res_len + 1 > res_cap) grow_results();
    results[res_len++] = v;
}

/* ------------------------------------------------------------------ */
/*   A 4‑digit primality lookup table, built on first use.             */
/* ------------------------------------------------------------------ */
static bool prime4[10000] = {0};
static bool prime4_inited = false;

/* ------------------------------------------------------------------ */
/*   The required OEIS function:  a(n)                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index below offset – undefined behavior */

    /* Ensure enough terms have been generated. */
    while (res_len < (size_t)n)
    {
        static uint64_t current = 2;   /* next integer to test          */
        static uint32_t window   = 0;  /* last up to 4 digits (as int)  */
        static uint32_t pend     = 0;  /* total digits seen so far      */

        /* Generate the next prime and append its digits to the stream. */
        if (isPrimeLarge(current))
        {
            /* add current to the prime list (used for future tests) */
            push_prime(current);

            /* process each digit of the prime              */
            uint64_t tmp = current;
            int   digs[20], dlen = 0;

            if (tmp == 0) digs[dlen++] = 0;   /* not needed for primes */

            while (tmp > 0)
            {
                digs[dlen++] = tmp % 10;     /* reverse order */
                tmp /= 10;
            }
            for (int i = dlen - 1; i >= 0; --i)
            {
                int d = digs[i];
                window = (window * 10 + d) % 10000;  /* keep last 4 digits */
                ++pend;
                if (pend >= 4)                 /* 4‑digit window ready */
                {
                    /* initialise 4‑digit prime table once          */
                    if (!prime4_inited)
                    {
                        for (int j = 0; j < 10000; ++j) prime4[j] = true;
                        prime4[0] = prime4[1] = false;
                        for (int j = 2; j * j < 10000; ++j)
                            if (prime4[j])
                                for (int k = j * j; k < 10000; k += j)
                                    prime4[k] = false;
                        prime4_inited = true;
                    }

                    if (prime4[window]) push_result(window);
                }
            }
        }
        ++current;
    }

    return (seq_t)results[n - 1];
}
