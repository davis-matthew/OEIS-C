/*
 * A051357  Chernoff sequence A006939 divided by 2.
 * https://oeis.org/A051357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (∏_{k=1..n} prime(k)^(n-k+1)) / 2
 * (the n‑th term of OEIS A051357)
 */
 
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*                     Prime generation (simple sieve)                 */
/* ------------------------------------------------------------------ */

#define SIEVE_LIMIT 200000          /* enough for n up to about 18000 */

static bool primes_initialized = false;
static uint32_t *primes = NULL;    /* primes[0] = 2, primes[1] = 3, ... */
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_initialized) return;

    bool *is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }

    /* Count primes to allocate the array */
    size_t cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    primes_cnt = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);
    primes_initialized = true;
}

/* ------------------------------------------------------------------ */
/*                     Sequence cache and helpers                       */
/* ------------------------------------------------------------------ */

static seq_t *prim_array = NULL;     /* vector of primorials, index 0 unused */
static seq_t *prod_array = NULL;     /* product of primorials up to i */
static size_t prim_cap = 0, prod_cap = 0;
static size_t prim_len = 0, prod_len = 0;

/* Ensure arrays are grown enough for at least 'n' entries (n >= 1) */
static void ensure_cache(idx_t n)
{
    if (n <= prim_len) return;        /* already computed */

    init_primes();

    /* grow capacity if needed */
    if ((size_t)n > prim_cap) {
        size_t new_cap = prim_cap ? prim_cap * 2 : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;

        prim_array = (seq_t*)realloc(prim_array, (new_cap + 1) * sizeof(seq_t));
        prod_array = (seq_t*)realloc(prod_array, (new_cap + 1) * sizeof(seq_t));
        prim_cap = prod_cap = new_cap;
    }

    /* initialize from the first term if this is the first call */
    if (prim_len == 0) {
        prim_array[0] = (seq_t)0;  /* unused slot for 0‑index */
        prod_array[0] = (seq_t)0;
    }

    /* compute missing terms */
    for (size_t i = prim_len + 1; i <= (size_t)n; ++i) {
        seq_t p = (seq_t)primes[i - 1];     /* primes[0] is 2 -> prime(1) */

        seq_t newPrim;
        if (i == 1)
            newPrim = p;
        else
            newPrim = prim_array[i - 1] * p;

        prim_array[i] = newPrim;

        seq_t newProd;
        if (i == 1)
            newProd = newPrim;
        else
            newProd = prod_array[i - 1] * newPrim;

        prod_array[i] = newProd;
    }

    prim_len = prod_len = (size_t)n;
}

/* ------------------------------------------------------------------ */
/*                               Main function                          */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return prod_array[n] / 2;   /* divide by 2 as the sequence definition */
}
