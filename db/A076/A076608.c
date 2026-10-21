/*
 * A076608  Number of nonprimes k < n such that also n-k is not a prime.
 * https://oeis.org/A076608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} [ !isPrime(k) && !isPrime(n-k) ]
 * Range:     u8 n <= 359, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (empty sequence is treated as non‑prime) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n);

/* ---------- static prime sieve ---------------------------------------- */
static bool *prime_sieve = NULL;   /* prime_sieve[i] == true  -> i is prime  */
static idx_t sieve_limit = 0;      /* highest number sieved so far */

/* Ensure that prime_sieve covers all numbers up to `limit`.  If
 * `limit` is larger than the current sieve, a new sieve is constructed
 * and the former is freed. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate new sieve array [0..limit] */
    bool *new_sieve = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!new_sieve) {
        /* Allocation failure – abort the program.  In the context of
           this library this should never happen for the expected limits. */
        abort();
    }

    /* Mark all numbers >= 2 as potential primes; 0 and 1 are not prime. */
    for (idx_t i = 2; i <= limit; ++i)
        new_sieve[i] = true;
    if (limit >= 0)
        new_sieve[0] = false;
    if (limit >= 1)
        new_sieve[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = false;
        }
    }

    /* Replace old sieve */
    free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit  = limit;
}

/* ---------- sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* a(1) is defined, a(0) can be safely 0 */
        return (seq_t)0;

    /* The counts involve numbers up to n-1 */
    ensure_sieve(n - 1);

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (!prime_sieve[k] && !prime_sieve[n - k])
            ++count;
    }
    return count;
}
