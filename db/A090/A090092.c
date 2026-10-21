/*
 * A090092  a(n) is the smallest composite number coprime to n, n+1 and n+2.
 * https://oeis.org/A090092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053671(n)^2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A053671(n) is the smallest prime p that is coprime to n, n+1, and n+2. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- Prime generation utilities ---------- */

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;
static uint64_t next_candidate = 3; /* next odd number to test for primality */

/* Ensure the prime array has enough capacity; double capacity when needed */
static void grow_prime_array(void)
{
    size_t new_cap = (prime_capacity == 0) ? 64 : prime_capacity * 2;
    uint64_t *new_ptr = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    prime_list = new_ptr;
    prime_capacity = new_cap;
}

/* Add a new prime to the list */
static void add_prime(uint64_t p)
{
    if (prime_count == prime_capacity) grow_prime_array();
    prime_list[prime_count++] = p;
}

/* Simple primality test using primes already in the list */
static int is_prime_candidate(uint64_t p)
{
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t pr = prime_list[i];
        if (pr * pr > p) break;
        if (p % pr == 0) return 0;
    }
    return 1;
}

/* Generate the next prime and append it to the list */
static void generate_next_prime(void)
{
    for (;;) {
        if (is_prime_candidate(next_candidate)) {
            add_prime(next_candidate);
            next_candidate += 2;
            break;
        }
        next_candidate += 2;
    }
}

/* Return the smallest prime p such that p does not divide n, n+1, n+2 */
static uint64_t smallest_coprime_prime(uint64_t n)
{
    /* Ensure prime list starts with 2 */
    if (prime_count == 0) {
        add_prime(2);          /* primes[0] = 2 */
        next_candidate = 3;    /* start testing odd numbers from 3 */
    }

    for (size_t idx = 0; ; ++idx) {
        if (idx >= prime_count) generate_next_prime();
        uint64_t p = prime_list[idx];

        if ((n % p) == 0) continue;
        if (((n + 1) % p) == 0) continue;
        if (((n + 2) % p) == 0) continue;

        return p;
    }
}

/* ---------- Sequence function ---------- */

seq_t a(idx_t n)
{
    /* Convert idx_t to uint64_t for arithmetic; safety if idx_t < 2^64 */
    uint64_t idx = (uint64_t)n;
    uint64_t p = smallest_coprime_prime(idx);
    return (seq_t)p * (seq_t)p;
}
