/*
 * A105469  Number of numbers of the form 6k+3 with prime(n) <= 6k+3 <
 *          prime(n+1).
 * https://oeis.org/A105469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integers of the form 6k+3 such that prime(n) <=
 *            6k+3 < prime(n+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_arr = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Ensure that prime_arr contains at least 'needed' primes
   (0‑based index).  The array grows on demand. */
static void
ensure_prime_count(idx_t needed)
{
    if (prime_count >= (size_t)needed)
        return;

    if (prime_cap < (size_t)needed) {
        prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        if (prime_cap < (size_t)needed)
            prime_cap = (size_t)needed;
        prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
    }

    /* Seed with the first prime. */
    if (prime_count == 0) {
        prime_arr[prime_count++] = 2;
        if (prime_count >= (size_t)needed)
            return;
    }

    uint64_t candidate = prime_arr[prime_count - 1] + 1;
    while (prime_count < (size_t)needed) {
        if (candidate % 2 == 0)
            ++candidate;                /* skip even numbers after 2 */

        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_arr[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
            prime_arr[prime_count++] = candidate;

        candidate += 2;                  /* next odd candidate */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                      /* behaviour for n < 1 is undefined */

    /* We need prime(n) and prime(n+1).  (primes are 1‑based in OEIS) */
    ensure_prime_count(n + 1);

    uint64_t p = prime_arr[n - 1];
    uint64_t q = prime_arr[n];

    /* First number ≥ p that is congruent to 3 modulo 6. */
    uint64_t rem   = p % 6;
    uint64_t offset = (3 + 6 - rem) % 6;
    uint64_t first = p + offset;

    if (first >= q)                     /* interval has no such number */
        return 0;

    uint64_t last = q - 1;               /* last number < q */
    uint64_t k_first = (first - 3) / 6;
    uint64_t k_last  = (last - 3) / 6;

    return (seq_t)(k_last - k_first + 1);
}
