/*
 * A191022  Primes that are squares mod 29.
 * https://oeis.org/A191022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p such that p is a quadratic residue modulo 29
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts: 5, 7, 13, 23, 53, 59, 67, 71, 83, 103, 107, 109, 139, 149, ... */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

static bool residues[29] = {0};
static uint64_t *primes = NULL;
static size_t p_cap = 0, p_len = 0;
static seq_t  *sol   = NULL;
static size_t sol_cap = 0, sol_len = 0;
static uint64_t candidate = 3;   /* next odd candidate > 2 */

/* Initializer for residues and prime list */
static void init(void)
{
    if (p_len) return;  /* already initialized */

    /* compute quadratic residues modulo 29 (excluding 0) */
    for (int i = 0; i < 29; ++i) residues[i] = false;
    for (int i = 1; i < 29; ++i) residues[(i * i) % 29] = true;

    /* prime list starts with 2 */
    p_cap = 4;
    primes = (uint64_t *)malloc(p_cap * sizeof(uint64_t));
    primes[0] = 2;
    p_len = 1;
}

/* test whether p is a quadratic residue modulo 29 */
static bool isQR(uint64_t p)
{
    if (p % 29 == 0) return false;
    return residues[p % 29];
}

/* simple trial‑division primality test using primes[] */
static bool isPrime(uint64_t x)
{
    if (x < 2) return false;
    uint64_t limit = (uint64_t)sqrt((double)x);
    for (size_t i = 0; i < p_len; ++i) {
        uint64_t d = primes[i];
        if (d > limit) break;
        if (x % d == 0) return false;
    }
    return true;
}

/* add prime to vector primes, growing if needed */
static void addPrime(uint64_t p)
{
    if (p_len == p_cap) {
        p_cap = p_cap * 2 + 1;
        primes = (uint64_t *)realloc(primes, p_cap * sizeof(uint64_t));
    }
    primes[p_len++] = p;
}

/* add result prime to vector sol, growing if needed */
static void addSol(uint64_t p)
{
    if (sol_len == sol_cap) {
        sol_cap = sol_cap * 2 + 1;
        sol = (seq_t *)realloc(sol, sol_cap * sizeof(seq_t));
    }
    sol[sol_len++] = (seq_t)p;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, undefined for n=0 */

    init();

    /* prepopulated first few terms (5 and 7) from sieve logic */
    while (sol_len < n) {
        if (candidate < 2) candidate = 2;

        /* check candidate for primality */
        if (isPrime(candidate)) {
            addPrime(candidate);
            if (isQR(candidate)) addSol(candidate);
        }
        ++candidate;
    }
    return sol[n - 1];
}
