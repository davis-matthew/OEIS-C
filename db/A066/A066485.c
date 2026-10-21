/*
 * A066485  Numbers n such that f(n) is a strict local extremum for the prime
 *          gaps function f(n) = prime(n+1)-prime(n), where prime(n) denotes
 *          the n-th prime; i.e., either f(n)>f(n-1) and f(n)>f(n+1) or
 *          f(n)<f(n-1) and f(n)<f(n+1).
 * https://oeis.org/A066485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th index for which the prime gap f(n) is a strict local extremum. */
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  static state and helper functions                                    */
/* --------------------------------------------------------------------- */
static uint64_t *primes   = NULL;
static size_t   primes_len = 0, primes_cap = 0;

static seq_t *gaps   = NULL;
static size_t  gaps_len = 0, gaps_cap = 0;

static seq_t *extrema   = NULL;
static size_t  extrema_len = 0, extrema_cap = 0;

/* append a new prime to the primes array */
static void append_prime(uint64_t p)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* append a new gap to the gaps array */
static void append_gap(seq_t g)
{
    if (gaps_len == gaps_cap) {
        size_t new_cap = gaps_cap ? gaps_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(gaps, new_cap * sizeof(seq_t));
        gaps = tmp;
        gaps_cap = new_cap;
    }
    gaps[gaps_len++] = g;
}

/* append a new extremum index to the extrema array */
static void append_extrema(seq_t v)
{
    if (extrema_len == extrema_cap) {
        size_t new_cap = extrema_cap ? extrema_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(extrema, new_cap * sizeof(seq_t));
        extrema = tmp;
        extrema_cap = new_cap;
    }
    extrema[extrema_len++] = v;
}

/* return the next prime greater than or equal to start.
   start is usually the last prime + 1. */
static uint64_t next_prime(uint64_t start)
{
    if (start <= 2) return 2;
    if (start % 2 == 0) ++start;          /* make odd */
    for (;;) {
        bool is_prime = true;
        uint64_t limit = (uint64_t) sqrt((double) start);
        for (size_t i = 0; i < primes_len && primes[i] <= limit; ++i) {
            if (start % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) return start;
        start += 2;                       /* skip even numbers */
    }
}

/* --------------------------------------------------------------------- */
/*  sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* offset is 1; behaviour for n==0 is irrelevant */
    if (n == 0) return 0;

    /* initialise with the first prime 2 */
    if (primes_len == 0) append_prime(2);

    /* generate primes and record extrema until we have n terms */
    while (extrema_len < (size_t)n) {
        /* compute next prime and its gap */
        uint64_t last_prime = primes[primes_len - 1];
        uint64_t nxt = next_prime(last_prime + 1);

        append_prime(nxt);
        seq_t gap = (seq_t)(nxt) - (seq_t)(last_prime);
        append_gap(gap);

        /* if we have at least three gaps, the middle of the last three
           can be checked for an extremum. */
        if (gaps_len >= 3) {
            size_t i = gaps_len - 2;          /* middle index */
            seq_t left  = gaps[i - 1];
            seq_t mid   = gaps[i];
            seq_t right = gaps[i + 1];
            if ((mid > left && mid > right) ||
                (mid < left && mid < right)) {
                append_extrema((seq_t)(i + 1));   /* gaps are 0‑based; sequence is 1‑based */
            }
        }
    }

    return extrema[n - 1];   /* 0‑based array index */
}
