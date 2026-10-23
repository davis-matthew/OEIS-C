/*
 * A171019  In the sequence of prime numbers, replace all the '4' digits with
 *          '0' and vice versa.
 * https://oeis.org/A171019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform the nth prime by swapping all '4' digits with
 *            '0' and vice versa
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic array of primes (stored as uint64_t) */
static uint64_t *prime_array = NULL;
static size_t prime_count   = 0;  /* number of primes stored */
static size_t prime_cap     = 0;  /* allocated capacity */
static uint64_t last_prime  = 1;  /* last prime generated */

/* enlarge the prime array when needed */
static void append_prime(uint64_t p)
{
    if (prime_count == prime_cap) {
        prime_cap = prime_cap ? prime_cap << 1 : 4;
        uint64_t *tmp = (uint64_t *)realloc(prime_array, prime_cap * sizeof(uint64_t));
        if (!tmp) { /* fallback – memory exhausted – never expected for n<=10000 */
            abort();
        }
        prime_array = tmp;
    }
    prime_array[prime_count++] = p;
}

/* generate the next prime and store it */
static void generate_next_prime(void)
{
    if (prime_count == 0) {
        append_prime(2);
        last_prime = 2;
        return;
    }

    uint64_t candidate = last_prime + 1;
    if ((candidate & 1) == 0)   /* skip even numbers after 2 */
        ++candidate;

    for (;; candidate += 2) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_array[i];
            if (q * q > candidate)
                break;
            if ((candidate % q) == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            append_prime(candidate);
            last_prime = candidate;
            break;
        }
    }
}

/* ensure that at least n primes are generated (1‑based index) */
static void ensure_primes(idx_t n)
{
    if (!prime_array) {
        prime_array = (uint64_t *)malloc(4 * sizeof(uint64_t));
        if (!prime_array) abort();
        prime_cap = 4;
        prime_count = 0;
        last_prime = 1;
    }
    while (prime_count < n)
        generate_next_prime();
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of domain – behaviour undefined for n=0 */

    ensure_primes(n);

    uint64_t p = prime_array[n - 1];   /* nth prime */

    /* transform the prime: swap 4<->0 */
    uint64_t rev = 0;
    uint64_t tmp = p;
    while (tmp > 0) {
        uint64_t d = tmp % 10;
        if (d == 0)          d = 4;
        else if (d == 4)     d = 0;
        rev = rev * 10 + d;
        tmp /= 10;
    }

    /* reverse back to original digit order, dropping leading zeros automatically */
    uint64_t result = 0;
    while (rev > 0) {
        result = result * 10 + rev % 10;
        rev /= 10;
    }

    return (seq_t)result;
}
