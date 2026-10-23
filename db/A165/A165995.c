/*
 * A165995  a(n) = Sum_{p > n} floor(n^2/p), for primes p.
 * https://oeis.org/A165995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p > n} floor(n^2 / p) , p prime
 * Range:     u8 n <= 15, u16 n <= 318, u32 n <= 2236, u64 n <= 2236,
 *            u128 n <= 2236, big any n
 * Helpers:   bit_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------  Includes --------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/bit_mask.h"

/* ---------------  Static data for sieving --------------- */
static uint8_t *sieve_bits    = NULL;  /* bitset: 0=prime, 1=composite */
static size_t  sieve_bits_size = 0;    /* bytes allocated for sieve_bits */
static idx_t   sieve_limit      = 0;    /* highest n fully sieved */

static uint32_t *primes_arr     = NULL; /* list of generated primes */
static size_t    primes_cap     = 0;     /* capacity of primes_arr */
static size_t    primes_used    = 0;     /* number of primes stored */

/* ---------------  Helpers for the bitset --------------- */
static inline size_t bit_index(idx_t n) { return n >> 3; }

static inline bool is_composite(idx_t n)
{
    return sieve_bits[bit_index(n)] & bit_mask(n);
}
static inline void set_composite(idx_t n)
{
    sieve_bits[bit_index(n)] |= bit_mask(n);
}

/* ---------------  Ensure sieve up to 'limit' --------------- */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;                /* already done */

    /* allocate/resize bitset */
    size_t new_size = (limit + 8) >> 3;
    if (!sieve_bits)
        sieve_bits = (uint8_t*)calloc(new_size, 1);
    else
    {
        sieve_bits = (uint8_t*)realloc(sieve_bits, new_size);
        if (!sieve_bits) abort();                    /* allocation failure */
    }
    if (sieve_bits_size < new_size)
        memset(sieve_bits + sieve_bits_size, 0, new_size - sieve_bits_size);
    sieve_bits_size = new_size;

    /* Sieve of Eratosthenes up to 'limit' */
    idx_t sqrt_limit = (idx_t)( (idx_t)( (double)limit ) *  (double)0.5 ); /* safe for limit <= 1e8 */
    for (idx_t i = 2; i <= sqrt_limit; ++i)
    {
        if (is_composite(i)) continue;
        idx_t j = i * i;
        for (; j <= limit; j += i)
            set_composite(j);
    }

    /* add new primes to the list */
    if (primes_arr == NULL)
    {
        primes_cap = 1024;
        primes_arr = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!primes_arr) abort();
    }
    for (idx_t n = sieve_limit + 1; n <= limit; ++n)
    {
        if (!is_composite(n))
        {
            if (primes_used == primes_cap)
            {
                primes_cap <<= 1;
                primes_arr = (uint32_t*)realloc(primes_arr, primes_cap * sizeof(uint32_t));
                if (!primes_arr) abort();
            }
            primes_arr[primes_used++] = (uint32_t)n;
        }
    }
    sieve_limit = limit;
}

/* ---------------  Main sequence function --------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* ensure primes up to n^2 are available */
    idx_t n2_idx = n * n;
    ensure_sieve(n2_idx);

    /* locate first prime > n */
    size_t lo = 0, hi = primes_used;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)primes_arr[mid] <= n) lo = mid + 1;
        else hi = mid;
    }
    size_t start = lo;

    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t sum = 0;

    for (size_t i = start; i < primes_used; ++i)
    {
        uint32_t p = primes_arr[i];
        if ((idx_t)p > n2_idx) break;   /* primes beyond n^2 not needed */
        sum += n2 / (seq_t)p;
    }
    return sum;
}
