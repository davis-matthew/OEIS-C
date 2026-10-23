/*
 * A139844  Primes of the form 2x^2 + 2xy + 53y^2.
 * https://oeis.org/A139844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A139844: primes of the form 2*x^2 + 2*x*y + 53*y^2
 * The sequence consists of all primes that are 2, 53, 113, 137,
 * 197, 233 or 317 modulo 420.  Using this characterisation
 * we generate primes with a sieve and filter them.  The cache
 * is updated lazily on each call to a(n).  The implementation
 * works for both built‑ins (unsigned int, uint64_t, …) and
 * arbitrary‑precision integer classes when compiled in C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*  The maximum value we sieve up to.  20 000 000 is comfortably
    large enough to contain more than 10 000 terms of the sequence. */
#define SIEVE_LIMIT 20000000UL

/* Residues modulo 420 that characterize the primes in the sequence. */
static const uint32_t residues[7] = {2, 53, 113, 137, 197, 233, 317};

/* membership test for the residue set */
static bool in_residue_set(uint64_t mod)
{
    for (size_t i = 0; i < 7; ++i)
        if (mod == residues[i])
            return true;
    return false;
}

/* Static storage for the sieve and prime list.  All memory is allocated
   the first time the function a() is called. */
static bool *is_composite = NULL;           /* sieve bitmap */
static uint32_t *primes = NULL;             /* list of primes up to SIEVE_LIMIT */
static size_t   primes_cnt  = 0;            /* number of primes stored */

/* Cache of sequence values.  We store them as uint64_t and cast to seq_t
   on return; this works for both builtin and arbitrary‑precision builds. */
#define CACHE_CAP 200000
static uint64_t sequence_cache[CACHE_CAP];
static size_t   cache_cnt      = 0;          /* number of cached terms */
static size_t   prime_ptr_idx  = 0;          /* index of next prime to test */

/* Build the sieve and the prime list.  This is done exactly once. */
static void build_sieve_and_primes(void)
{
    /* Allocate sieve bitmap */
    is_composite = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_composite)
        abort();   /* allocation failure */
    memset(is_composite, 0, (SIEVE_LIMIT + 1) * sizeof(bool));
    is_composite[0] = is_composite[1] = true;

    /* Simple Eratosthenes sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_composite[j] = true;

    /* Allocate array for primes (over‑allocate to avoid reallocation). */
    primes = (uint32_t *)malloc((SIEVE_LIMIT / 10) * sizeof(uint32_t));  /* ~1/10 of limit */
    if (!primes)
        abort();

    /* Collect primes into the array */
    primes_cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            primes[primes_cnt++] = i;
}

/* Public function: a(n) returns the nth term of the sequence for n >= 1. */
seq_t a(idx_t n)
{
    /* Ensure sieve and prime list are prepared once */
    static bool initialized = false;
    if (!initialized) {
        build_sieve_and_primes();
        initialized = true;
    }

    /* Guarantee n is within cache bounds */
    if (n < 1 || (size_t)n > CACHE_CAP)
        abort();   /* out of range for this simple implementation */

    /* If cache already contains the requested term, return it. */
    if ((size_t)n <= cache_cnt)
        return (seq_t)sequence_cache[n - 1];

    /* Generate terms until we reach the required index. */
    while ((size_t)n > cache_cnt && prime_ptr_idx < primes_cnt) {
        uint64_t p = primes[prime_ptr_idx++];
        if (in_residue_set(p % 420ULL)) {
            if (cache_cnt >= CACHE_CAP)
                abort();   /* cache overflow – unlikely for n <= 10000 */
            sequence_cache[cache_cnt++] = p;
        }
    }

    /* If we reached the end of the prime list before finding enough terms,
       the sieve limit was insufficient.  In the present context this should
       never happen for n up to 10 000.  */
    if ((size_t)n > cache_cnt)
        abort();

    return (seq_t)sequence_cache[n - 1];
}
