/*
 * A104492  Cube excess of the n-th prime.
 * https://oeis.org/A104492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - floor(prime(n)^(1/3))^3
 * Range:     u8 n <= 47, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   cube_root_floor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the cube excess of the n‑th prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/cube_root_floor.h"

/* Static buffer for primes */
static uint64_t *primes = NULL;  /* array of primes */
static idx_t prime_cnt = 0;      /* number of primes stored */
static idx_t prime_cap = 0;      /* capacity of the array */

/* Grow the primes array to be able to store at least 'n' primes. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Grow capacity if needed. */
    if (prime_cap < n) {
        idx_t new_cap = prime_cap ? prime_cap * 2 : 64;
        while (new_cap < n)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) {
            /* Allocation failure – terminate. */
            exit(1);
        }
        primes = tmp;
        prime_cap = new_cap;
    }

    /* Find primes up to the required count. */
    uint64_t candidate = 2;
    if (prime_cnt) {
        candidate = primes[prime_cnt - 1] + 1;
    }

    /* Simple trial division using the is_prime utility. */
    for ( /* use candidate from above */; prime_cnt < n; candidate++) {
        if (is_prime(candidate)) {
            primes[prime_cnt++] = candidate;
        }
    }
}

/* Return the cube excess of the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1, return 0 safely */

    ensure_primes(n);
    uint64_t p = primes[n - 1];    /* nth prime (1‑based) */

    seq_t sp = (seq_t)p;           /* promote to seq_t type */
    seq_t r = cube_root_floor(sp); /* floor cube root of sp */
    seq_t cube = r * r * r;

    return sp - cube;              /* cube excess */
}
