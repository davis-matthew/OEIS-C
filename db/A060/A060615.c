/*
 * A060615  Number of conjugacy classes in the group GL_2(K) when K is a
 *          finite field with q = p^m for a prime p and m >= 1.
 * https://oeis.org/A060615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000961(n+2)^2 - 1
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the square of the (n+2)-nd prime power (1 counted), minus 1 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for generated prime powers and primes */
static uint64_t *prime_powers = NULL;   /* prime powers, 0‑based (prime_powers[0] = 1) */
static size_t pp_count = 0, pp_cap = 0;
static uint64_t *primes = NULL;         /* prime numbers for factorisation */
static size_t prime_count = 0, prime_cap = 0;
static uint64_t next_candidate = 2;      /* next integer to test for prime power */

/* initialise the storage structures on first use */
static void init_storage(void)
{
    if (!prime_powers) {
        pp_cap = 8;
        pp_count = 1;
        prime_powers = (uint64_t *)malloc(pp_cap * sizeof(uint64_t));
        if (!prime_powers) return;          /* allocation failed – further growth aborted */
        prime_powers[0] = 1;                /* 1 is the first prime power in A000961 */

        prime_cap = 16;
        prime_count = 0;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
}

/* test whether x (>=2) is a prime power; uses the current list of primes */
static bool is_prime_power(uint64_t x)
{
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) {
            uint64_t temp = x;
            while (temp % p == 0)
                temp /= p;
            return temp == 1;               /* only one distinct prime factor found */
        }
    }
    /* no small prime factor – x is prime, thus a prime power */
    return true;
}

/* ensure that prime_powers[] contains all prime powers up to index 'index' (0‑based) */
static void ensure_prime_powers_up_to_index(size_t index)
{
    init_storage();

    while (pp_count <= index) {
        uint64_t cand = next_candidate++;

        /* test whether cand is prime */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* add to primes list */
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!primes) return;           /* allocation failed – abort further growth */
            }
            primes[prime_count++] = cand;
        }

        /* prime powers include primes and pure powers of a single prime */
        if (is_prime || is_prime_power(cand)) {
            if (pp_count == pp_cap) {
                pp_cap = pp_cap ? pp_cap * 2 : 8;
                prime_powers = (uint64_t *)realloc(prime_powers, pp_cap * sizeof(uint64_t));
                if (!prime_powers) return;     /* allocation failed – abort further growth */
            }
            prime_powers[pp_count++] = cand;
        }
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    /* For a(n) we need the (n+2)-nd prime power.
       Since prime_powers[0] = 1 (the 1st prime power), the desired element
       is at zero‑based index n+1. */
    size_t target_index = (size_t)n + 1;           /* zero‑based */
    ensure_prime_powers_up_to_index(target_index);

    uint64_t q = prime_powers[target_index];       /* the prime power itself */
    seq_t prime = (seq_t)q;                        /* cast to the sequence type */
    seq_t result = prime * prime - 1;              /* q^2 - 1 */

    return result;
}
