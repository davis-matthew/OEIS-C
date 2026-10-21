/*
 * A084245  a(n) = (n + (prime(n) mod 7) - 3) mod 10.
 * https://oeis.org/A084245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + (prime(n) mod 7) - 3) mod 10
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static idx_t  *prime_list   = NULL;   /* array of discovered primes */
static size_t  prime_count  = 0;      /* number of primes stored     */
static size_t  prime_cap    = 0;      /* current capacity            */
static idx_t   next_candidate = 3;    /* next number to test (odd only) */

/* grow the prime array when needed */
static void ensure_prime_capacity(size_t required)
{
    if (required <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < required) new_cap = required;
    prime_list = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    prime_cap = new_cap;
}

/* test if x is prime using trial division by previously found primes */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i)
    {
        idx_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* generate primes until we have at least 'needed' primes */
static void generate_primes_up_to(size_t needed)
{
    if (prime_count == 0)
    {
        /* initialize with first prime 2 */
        ensure_prime_capacity(needed);
        prime_list[prime_count++] = 2;
        next_candidate = 3;
    }
    while (prime_count < needed)
    {
        while (!is_prime(next_candidate))
            next_candidate += 2;
        ensure_prime_capacity(prime_count + 1);
        prime_list[prime_count++] = next_candidate;
        next_candidate += 2;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* 1‑based indexing */
    if (n == 0) return 0; /* behaviour for n<1 is irrelevant */

    generate_primes_up_to((size_t)n); /* ensure prime(n) known */
    idx_t pn = prime_list[n - 1];     /* prime(n) */
    seq_t res = ((seq_t)n + (seq_t)(pn % 7) + 7) % 10; /* (n + pn%7 - 3) mod 10 */
    return res;
}
