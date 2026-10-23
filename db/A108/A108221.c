/*
 * A108221  Primes of the form ceiling(sqrt(prime(n))).
 * https://oeis.org/A108221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108221: Primes of the form ceil(sqrt(prime(n))).
   Computes the nth element by iterating the primes in
   increasing order, emitting ceil(sqrt(p_k)) whenever that
   value is itself prime.  Duplicates occur when different
   primes share the same ceil(sqrt) value.  The implementation
   generates primes up to a fixed upper bound (4 000 000)
   which is sufficient to produce at least 10 000 terms,
   and expands the output array on demand.  */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* Limits for the sieve (4 000 000 is ample for 10 000 terms). */
#define MAX_PRIME_LIMIT   4000000
/* Initial capacities for dynamic arrays. */
#define INIT_PRIME_CAP    300000
#define INIT_OUTPUT_CAP   64

static bool    *is_prime_arr   = NULL;  /* sieve array */
static uint32_t *prime_arr     = NULL;  /* list of primes */
static seq_t  *output_arr      = NULL;  /* output of sequence A108221 */
static idx_t   prime_cnt       = 0;     /* number of primes found */
static idx_t   output_size     = 0;     /* number of output terms generated */
static idx_t   output_cap      = 0;     /* capacity of output_arr */
static idx_t   prime_index     = 0;     /* next prime to process */
static bool    initialized     = false;

/* ------------------------------------------------------------------ */
/* Integer square‑root (floor) using binary search.  Works for
   unsigned 64‑bit values up to MAX_PRIME_LIMIT.                     */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = 3000000ULL;  /* > sqrt(4 000 000)   */
    while (lo <= hi)
    {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned long long sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;    /* floor sqrt */
}

/* Ceil of square root for a 64‑bit unsigned integer. */
static inline unsigned long long ceil_sqrt_u64(unsigned long long x)
{
    unsigned long long s = isqrt_u64(x);
    if (s * s < x) ++s;
    return s;
}

/* Ensure the sieve is performed once and primes are listed. */
static void init_sieve(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate sieve array. */
    is_prime_arr = (bool*)malloc((MAX_PRIME_LIMIT + 1) * sizeof(bool));
    memset(is_prime_arr, true, (MAX_PRIME_LIMIT + 1) * sizeof(bool));
    is_prime_arr[0] = is_prime_arr[1] = false;

    for (uint32_t i = 2; i * (uint64_t)i <= MAX_PRIME_LIMIT; ++i)
    {
        if (is_prime_arr[i])
            for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_prime_arr[j] = false;
    }

    /* Allocate array for primes. */
    prime_arr = (uint32_t*)malloc(INIT_PRIME_CAP * sizeof(uint32_t));
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (is_prime_arr[i])
        {
            if (prime_cnt == INIT_PRIME_CAP)
            {
                /* Resize if needed (unlikely for 4 000 000). */
                uint32_t new_cap = (prime_cnt + 1) * 2;
                prime_arr = (uint32_t*)realloc(prime_arr, new_cap * sizeof(uint32_t));
            }
            prime_arr[prime_cnt++] = i;
        }

    /* Allocate output array. */
    output_arr = (seq_t*)malloc(INIT_OUTPUT_CAP * sizeof(seq_t));
    output_cap = INIT_OUTPUT_CAP;
}

/* Append a new term to the output array, resizing if necessary. */
static void append_output(seq_t value)
{
    if (output_size == output_cap)
    {
        size_t new_cap = output_cap * 2;
        seq_t *tmp = (seq_t*)realloc(output_arr, new_cap * sizeof(seq_t));
        output_arr = tmp;
        output_cap = new_cap;
    }
    output_arr[output_size++] = value;
}

/* Generate terms until at least n terms are available. */
static void generate_up_to(idx_t n)
{
    while (output_size < n)
    {
        if (prime_index >= prime_cnt)
            break;  /* safety: no more primes in the sieve */

        uint32_t p = prime_arr[prime_index++];
        unsigned long long r = ceil_sqrt_u64((unsigned long long)p);
        /* r is ≤ sqrt(MAX_PRIME_LIMIT) ≤ 2000, inside is_prime_arr. */
        if (is_prime_arr[(uint32_t)r])
            append_output((seq_t)r);
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the specification. */
seq_t a(idx_t n)
{
    init_sieve();
    if (n == 0) return (seq_t)0;
    if (n > output_size)
        generate_up_to(n);
    if (n <= output_size)
        return output_arr[n - 1];
    /* If we ran out of primes in the sieve (unlikely), return 0. */
    return (seq_t)0;
}
