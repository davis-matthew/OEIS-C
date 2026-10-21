/*
 * A060727  For n >= 1 a(n) is the number of permutations in the symmetric
 *          group S_n such that their cycle decomposition contains no 7-cycle.
 * https://oeis.org/A060727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{i=0..⌊n/7⌋} ((-1)^i) / (i! * 7^i)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expected types:  seq_t is an integer type (unsigned *-bit or arbitrary-precision).
 *                  idx_t is an unsigned integer type (uint64_t or equivalent). */

#include <stdlib.h>

/* ---------- Factorial cache ----------
 * fact_cache[0] = 0! = 1
 * fact_cache[k] = k!
 */

/* Global static cache (shared between helper functions) */
static seq_t *fact_cache = NULL;   /* array of factorials */
static idx_t  fact_cache_size = 0; /* number of elements stored (size of array) */

/* Ensure factorials up to n are computed and cached */
static void ensure_factorial(idx_t n)
{
    if (n < fact_cache_size)                /* already have enough */
        return;

    /* New size: n+1 (need to store up to n) */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, sizeof(seq_t) * new_size);
    if (!new_cache) {                        /* Should never happen on these systems */
        exit(1);
    }

    /* If first time, set 0! = 1 */
    if (fact_cache_size == 0) {
        new_cache[0] = (seq_t)1;
        fact_cache_size = 1;
    }

    /* Compute missing factorials */
    for (idx_t i = fact_cache_size; i < new_size; ++i) {
        new_cache[i] = new_cache[i-1] * (seq_t)i;
    }

    fact_cache = new_cache;
    fact_cache_size = new_size;
}

/* Retrieve cached factorial value */
static seq_t get_factorial(idx_t n)
{
    return fact_cache[n];
}

/* Core function required by the task */
seq_t a(idx_t n)
{
    /* Ensure factorial up to n is cached */
    ensure_factorial(n);
    seq_t fact = get_factorial(n);

    /* Alt sum: term_i = n! / (i! * 7^i) */
    seq_t term = fact;
    seq_t sum  = 0;
    idx_t maxk = n / 7;          /* floor(n/7) */

    /* Iterate i = 0 .. maxk */
    for (idx_t i = 0; i <= maxk; ++i) {
        if (i & 1)
            sum -= term;
        else
            sum += term;

        /* Prepare next term: term = term / ((i+1)*7) */
        if (i < maxk) {
            seq_t divisor = ((seq_t)(i + 1)) * 7u;
            term = term / divisor;
        }
    }
    return sum;
}
