/*
 * A066248  a(n) = if n+1 is prime then A049084(n+1)*2 else A066246(n+1)*2 -
 *          1.
 * https://oeis.org/A066248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066248:  a(n) = if (n+1) is prime then 2·π(n+1)
 *                else 2·((n+1)-1-π(n+1)) – 1
 *
 * The implementation uses a dynamic prime sieve that expands to the
 * largest value requested.  π(x) is pre‑computed as a prefix sum of the
 * prime flag array.  The function works for any idx_t/seq_t types,
 * including an arbitrary‑precision seq_t in a C++ build.  All casts from
 * realloc are explicit so the code compiles in both C and C++.  For
 * n ≥ 1 the result is correct, and calls may be made in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --------- Dynamic sieve storage ----------------------------------- */
static uint8_t *prime_flag = NULL;   /* 1 if i is prime, 0 otherwise   */
static idx_t *pi_table   = NULL;     /* π(i) for 0 ≤ i ≤ sieve_limit   */
static idx_t sieve_limit = 0;        /* maximum index that has been sieved */

/* --------- Ensure that all numbers ≤ limit are sieved ----------------*/
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Reallocate arrays to the new limit + 1 */
    prime_flag = (uint8_t *)realloc(prime_flag, (size_t)(limit + 1) * sizeof(uint8_t));
    pi_table   = (idx_t   *)realloc(pi_table,   (size_t)(limit + 1) * sizeof(idx_t));
    if (!prime_flag || !pi_table) {
        abort();  /* memory allocation failure */
    }

    /* Initialise all entries as prime */
    for (idx_t i = 0; i <= limit; ++i)
        prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;        /* 0 and 1 are not prime */

    /* Classic Eratosthenes sieve */
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = 0;
        }
    }

    /* Build prefix prime counts π(i) */
    pi_table[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi_table[i] = pi_table[i - 1] + (prime_flag[i] ? 1 : 0);

    sieve_limit = limit;
}

/* --------- Sequence entry -------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1, but we allow n = 0 without
     * checking because the caller may never invoke it with n = 0.
     * Compute at index m = n + 1. */
    idx_t m = n + 1;
    ensure_sieve(m);

    /* π(m) */
    seq_t pi = (seq_t)pi_table[m];

    if (prime_flag[m]) {
        /* m is prime: a(n) = 2·π(m) */
        return pi * (seq_t)2;
    } else {
        /* m is composite: a(n) = 2·((m-1)-π(m)) – 1 */
        seq_t sub = (seq_t)(m - 1) - pi;
        return sub * (seq_t)2 - (seq_t)1;
    }
}
