/*
 * A008348  a(0)=0; thereafter a(n) = a(n-1) + prime(n) if a(n-1) < prime(n),
 *          otherwise a(n) = a(n-1) - prime(n).
 * https://oeis.org/A008348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 6453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008348
   Formula: a(0)=0; for n≥1,
      a(n) = a(n‑1) + prime(n)   if a(n‑1) < prime(n)
      a(n) = a(n‑1) – prime(n)   otherwise
   Offset: 0
   The routine builds both the sequence values and the list of primes
   lazily, so calls may occur in any order.
   It uses only integer arithmetic and does not pre‑compute a table of
   values.  The implementation is O(n) per new term and works for n
   up to at least 10 000 in under a second.  For arbitrary‑precision
   builds seq_t behaves like a C++ integer or such big‑int class.  Any
   unsigned type that supports +, -, <, and type‑cast is accepted.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers for prime generation ------------------------------------ */
static idx_t *prime_list   = NULL; /* 0‑based: prime_list[0] = 2 */
static idx_t  prime_count  = 0;    /* number of primes stored   */
static idx_t  prime_cap    = 0;    /* allocated capacity        */

static void grow_prime_cap(void)
{
    if (prime_cap == 0) { prime_cap = 16; }
    else                { prime_cap *= 2; }
    prime_list = (idx_t *)realloc(prime_list,
                                  prime_cap * sizeof(idx_t));
    if (!prime_list) abort();
}

static void add_prime(idx_t p)
{
    if (prime_count == prime_cap) grow_prime_cap();
    prime_list[prime_count++] = p;
}

/* Generate the next prime after the current list.                */
static void generate_next_prime(void)
{
    idx_t candidate;
    if (prime_count == 0) {
        candidate = 2;
    } else {
        candidate = prime_list[prime_count - 1] + 1;
        if ((candidate & 1) == 0) ++candidate; /* force odd */
    }

    for (;;) {
        bool is_prime = true;
        for (idx_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) { add_prime(candidate); break; }
        candidate += 2; /* skip even numbers */
    }
}

/* Ensure that prime_list contains at least 'n' primes (1‑based). */
static void ensure_primes(idx_t n)
{
    while (prime_count < n) generate_next_prime();
}

/* Main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    /* Cache for the sequence values. */
    static seq_t *cache      = NULL; /* cache[i] = a(i)   */
    static idx_t  cache_size = 0;    /* number of cached terms */

    if (n >= cache_size) {          /* need to grow cache      */
        idx_t new_size = n + 1;      /* indices 0 … n inclusive*/
        cache = (seq_t *)realloc(cache,
                                  new_size * sizeof(seq_t));
        if (!cache) abort();

        /* Initialisation for a(0). */
        if (cache_size == 0) cache[0] = 0;

        /* Make sure enough primes exist. */
        ensure_primes(n);

        /* Compute any missing terms. */
        idx_t start = cache_size;
        if (start == 0) start = 1;   /* a(0) already known */
        for (idx_t i = start; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t p    = (seq_t)prime_list[i - 1]; /* prime(i) */
            if (prev < p)
                cache[i] = prev + p;
            else
                cache[i] = prev - p;
        }

        cache_size = new_size;
    }

    return cache[n];
}
