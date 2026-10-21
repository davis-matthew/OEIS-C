/*
 * A076079  Largest multiple of n < the n-th prime.
 * https://oeis.org/A076079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n - 1) / n) * n (p_n = n-th prime)
 * Range:     u8 n <= 54, u16 n <= 6552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Prime generation helpers ---------- */
static seq_t *prime_list = NULL;      /* dynamic array of primes */
static size_t  prime_cnt  = 0;        /* number of primes stored   */
static size_t  prime_cap  = 0;        /* array capacity            */
static seq_t   last_prime = 1;        /* largest prime computed so far */

/* Add a new prime to the table, resizing if needed */
static void add_prime(seq_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        seq_t *new_list = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!new_list) exit(EXIT_FAILURE);   /* Out of memory */
        prime_list = new_list;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Check if x is prime using the already known primes. */
static int is_prime_seq(seq_t x)
{
    if (x < 2) return 0;
    if (x == 2) return 1;
    if ((x & 1) == 0) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Return the n-th prime (1‑based). Generates primes on demand. */
static seq_t get_prime(idx_t n)
{
    while (prime_cnt < n) {
        ++last_prime;
        if (is_prime_seq(last_prime))
            add_prime(last_prime);
    }
    return prime_list[n - 1];
}

/* ---------- OEIS sequence a(n) ---------- */
seq_t a(idx_t n)
{
    seq_t prime = get_prime(n);          /* p_n */
    seq_t k = (prime - (seq_t)1) / (seq_t)n;  /* floor((p_n-1)/n) */
    return k * (seq_t)n;                 /* largest multiple of n below p_n */
}
