/*
 * A139168  a(n) = (prime(n)! + 10)/10.
 * https://oeis.org/A139168
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)! + 10) / 10
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   ensure_capacity, get_prime, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/ensure_capacity.h"
#include "../utils/seq_one.h"

static seq_t *facts = NULL;          /* factorial of prime(1..k) stored 0‑based */
static size_t facts_cap = 0;         /* capacity of facts */
static idx_t facts_size = 0;         /* number of entries stored */
static seq_t current_factorial = 1;  /* factorial of the last processed prime */
static uint64_t last_prime_val = 0;  /* value of the last processed prime */

seq_t a(idx_t n)
{
    /* For indices below the offset, return 0; behaviour is undefined otherwise */
    if (n < 3)
        return 0;

    /* Allocate the facts array if first call or grow it enough */
    if (facts_size < n) {
        size_t new_cap = facts_cap ? facts_cap : 8;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        ensure_capacity(new_cap, &facts_cap, (void **)&facts, sizeof(seq_t));
    }

    /* If we have already computed up to n, just use the cached factorial */
    if (facts_size >= n) {
        seq_t fac = facts[n - 1];
        return (fac + (seq_t)10) / (seq_t)10;
    }

    /* Compute factorials for the remaining primes up to n */
    for (idx_t idx = facts_size + 1; idx <= n; ++idx) {
        /* Obtain the prime for this index */
        uint32_t p = get_prime(idx);

        /* Multiply through all integers from last_prime_val+1 up to p */
        for (idx_t x = last_prime_val + 1; x <= (idx_t)p; ++x)
            current_factorial = current_factorial * (seq_t)x;

        /* Store the factorial of the current prime */
        facts[idx - 1] = current_factorial;
        last_prime_val = p;
    }

    facts_size = n;
    seq_t fac = facts[n - 1];
    return (fac + (seq_t)10) / (seq_t)10;
}
