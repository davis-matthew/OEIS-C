/*
 * A161164  Absolute value of (the n-th twin prime minus the n-th non-twin
 *          prime).
 * https://oeis.org/A161164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |A001097(n)-A007510(n)| (twin prime members list A001097
 *            and non‑twin prime list A007510)
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* static state                                                          */
/* --------------------------------------------------------------------- */
static bool    init_done   = false;
static size_t  sieve_limit = 2000000; /* sufficient for n ≤ 10000 */
static uint8_t *prime_flag = NULL;    /* 1=prime, 0=composite */
static uint32_t *prime_list = NULL;
static size_t   prime_count = 0;
static uint32_t *twin_list  = NULL;
static size_t   twin_count  = 0;
static uint32_t *non_list   = NULL;
static size_t   non_count   = 0;

/* --------------------------------------------------------------------- */
static void init_sequences(void)
{
    if (init_done)
        return;

    /* ---------- sieve of Eratosthenes ------------------------------- */
    prime_flag = (uint8_t *)malloc((sieve_limit + 1) * sizeof(uint8_t));
    for (size_t i = 0; i <= sieve_limit; ++i)
        prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;
    for (size_t p = 2; p * p <= sieve_limit; ++p) {
        if (prime_flag[p]) {
            for (size_t m = p * p; m <= sieve_limit; m += p)
                prime_flag[m] = 0;
        }
    }

    /* ---------- collect all primes --------------------------------- */
    prime_count = 0;
    size_t cap = sieve_limit / 10 + 1;           /* rough capacity */
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    for (size_t n = 2; n <= sieve_limit; ++n) {
        if (prime_flag[n]) {
            if (prime_count == cap) {
                cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = (uint32_t)n;
        }
    }

    /* ---------- build twin and non‑twin lists ------------------------ */
    twin_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    non_list  = (uint32_t *)malloc(prime_count * sizeof(uint32_t));

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        bool has_twin = false;

        if (p > 2 && prime_flag[p - 2])
            has_twin = true;              /* partner below */
        if ((size_t)p + 2 <= sieve_limit && prime_flag[p + 2])
            has_twin = true;              /* partner above */

        if (has_twin)
            twin_list[twin_count++] = p;
        else
            non_list[non_count++] = p;
    }

    init_done = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n=0, return 0 safely */
        return (seq_t)0;

    init_sequences();

    idx_t idx = n - 1;          /* convert to zero‑based index */

    if ((size_t)idx >= twin_count || (size_t)idx >= non_count)
        return (seq_t)0;        /* beyond precomputed data */

    seq_t t  = (seq_t)twin_list[idx];
    seq_t nt = (seq_t)non_list[idx];
    return (t >= nt) ? (t - nt) : (nt - t);
}
