/*
 * A179278  Largest nonprime integer <= n.
 * https://oeis.org/A179278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179278 – largest nonprime integer ≤ n.  
   The function uses a prime sieve (Eratosthenes) up to the current
   largest query and then scans downwards from n until a non‑prime is
   found.  The sieve is rebuilt only when a larger n is requested. */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static idx_t sieve_limit = 0;      /* largest n already sieved */
static bool *is_prime_arr = NULL;  /* primality table, 0..limit */

/* Rebuild the prime sieve up to 'limit' (inclusive). */
static void ensure_sieve(idx_t limit) {
    if (limit <= sieve_limit)
        return;

    /* Resize the array.  Cast is needed for C++ compatibility. */
    bool *new_prime = (bool *)realloc(is_prime_arr, (limit + 1) * sizeof(bool));
    if (!new_prime) {
        abort();                       /* out of memory; terminate */
    }
    is_prime_arr = new_prime;

    /* Initialise all entries to true, then mark composites. */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime_arr[i] = true;
    if (limit >= 0) is_prime_arr[0] = false;
    if (limit >= 1) is_prime_arr[1] = false;

    idx_t root = (idx_t)sqrt((long double)limit);
    for (idx_t p = 2; p <= root; ++p) {
        if (is_prime_arr[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime_arr[m] = false;
        }
    }

    sieve_limit = limit;
}

/* External function required by the evaluator. */
seq_t a(idx_t n) {
    if (n == 0)          /* sequence is defined for n ≥ 1; 0 is defensive */
        return (seq_t)0;

    if (n == 1)          /* 1 is the smallest nonprime */
        return (seq_t)1;

    ensure_sieve(n);

    for (idx_t i = n; i >= 2; --i) {
        if (!is_prime_arr[i])
            return (seq_t)i;   /* largest nonprime ≤ n */
    }
    /* Should never reach here because 1 is nonprime */
    return (seq_t)1;
}
