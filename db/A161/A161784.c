/*
 * A161784  Product of primes on successive square intervals.
 * https://oeis.org/A161784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{p prime, n^2 < p < (n+1)^2} p * The
 *            implementation builds a bit sieve up to (n+1)^2, then multiplies
 *            * all primes in the interval. A single static sieve is reused
 *            across * calls; it is rebuilt whenever a larger bound is
 *            required
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* ------------------------------------------------------------------ */
/*  Dynamic bit sieve                                                */
/* ------------------------------------------------------------------ */

static unsigned char *sieve_bits = NULL; /* bitset, bit 0 => number 0      */
static size_t        sieve_max  = 0;      /* highest sieved number (inclusive) */

/* Test whether x is composite: 1 => composite, 0 => prime or < 2 */
static inline int is_composite(size_t x)
{
    return (sieve_bits[x >> 3] & (1u << (x & 7))) != 0;
}

/* Extend the sieve up to 'limit' (inclusive).  If limit <= current
 * maximum, nothing is done.  The function frees the old buffer and
 * rebuilds a new one from scratch; this is simple and fast enough
 * for the intended range (n <= 10000 -> limit <= 10^8).            */
static void build_sieve_up_to(size_t limit)
{
    if (limit <= sieve_max)            /* already sufficient */
        return;

    free(sieve_bits);

    size_t byte_len = (limit >> 3) + 1;               /* number of bytes needed */
    sieve_bits = (unsigned char*)malloc(byte_len);    /* cast required in C++ */
    if (!sieve_bits)                                 /* allocation failed */
        exit(1);                                      /* abort – cannot proceed */

    /* clear all bits (0: composite, 1: prime placeholder) */
    for (size_t i = 0; i < byte_len; ++i)
        sieve_bits[i] = 0;

    /* mark 0 and 1 as composite */
    sieve_bits[0] = 3;        /* bits 0 and 1 set (0b00000011) */

    /* classic sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (!is_composite(i))
        {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_bits[j >> 3] |= 1u << (j & 7);
        }
    }

    sieve_max = limit;
}

/* ------------------------------------------------------------------ */
/*  Sequence a(n)                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure n >= 1 (OEIS offset).  Under no circumstances is a(0)
     * evaluated by the tests, but the check makes the function safer
     * if used elsewhere. */
    assert(n >= 1);

    /* Bounds of the square interval: (n^2, (n+1)^2) */
    size_t start = (size_t)n * (size_t)n + 1;
    size_t stop  = (size_t)(n + 1) * (size_t)(n + 1) - 1;

    /* Build or extend the sieve up to 'stop' */
    build_sieve_up_to(stop);

    /* Multiply all primes p in the interval */
    seq_t prod = 1;
    for (size_t x = start; x <= stop; ++x)
    {
        if (!is_composite(x))
            prod *= (seq_t)x;
    }

    return prod;
}
