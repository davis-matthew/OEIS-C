/*
 * A046503  Primes with multiplicative persistence value 3.
 * https://oeis.org/A046503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Search for primes whose multiplicative persistence is 3.
 * The function builds primes incrementally, testing each prime for
 * persistence 3, and stores the qualifying primes.
 * Memoized arrays are used so calls can arrive in any order.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- helper structures ---------- */
static uint64_t *prime_list = NULL;     /* all primes found */
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;

static uint64_t *pp3_list = NULL;      /* primes with persistence 3 */
static size_t pp3_cnt  = 0;
static size_t pp3_cap  = 0;

static uint64_t next_candidate = 2;    /* next odd number to test */

/* --------- dynamic array helpers ---------- */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 4 : prime_cap * 2;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

static void add_pp3(uint64_t p)
{
    if (pp3_cnt == pp3_cap) {
        size_t new_cap = pp3_cap == 0 ? 4 : pp3_cap * 2;
        uint64_t *tmp = (uint64_t*)realloc(pp3_list, new_cap * sizeof(uint64_t));
        pp3_list = tmp;
        pp3_cap  = new_cap;
    }
    pp3_list[pp3_cnt++] = p;
}

/* --------- digit product and persistence ---------- */
static uint64_t prod_digits(uint64_t n)
{
    uint64_t prod = 1;
    while (n > 0) {
        prod *= n % 10;
        n /= 10;
    }
    return prod;
}

static bool has_persistence3(uint64_t n)
{
    int steps = 0;
    while (n >= 10) {
        n = prod_digits(n);
        steps++;
        if (steps > 3) return false;
    }
    return steps == 3;
}

/* --------- primality test by trial division ---------- */
static bool is_prime_candidate(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* --------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, guard only */

    /* generate primes until we have at least n pp3 primes */
    while (pp3_cnt < (size_t)n) {
        uint64_t p;

        /* obtain next prime */
        if (next_candidate == 2) {
            p = 2;
            next_candidate = 3;
        } else {
            while (true) {
                if (is_prime_candidate(next_candidate)) {
                    p = next_candidate;
                    next_candidate += 2;  /* next odd number */
                    break;
                }
                next_candidate += 2;
            }
        }

        add_prime(p);

        if (has_persistence3(p)) {
            add_pp3(p);
        }
    }

    return (seq_t)pp3_list[n - 1];
}
