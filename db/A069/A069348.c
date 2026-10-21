/*
 * A069348  Primes of the form k + Omega(k), where Omega(k) is the number of
 *          prime factors of k, A001222(k).
 * https://oeis.org/A069348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p for which there exists a k such that p
 *            = k + Ω(k), where Ω(k) is the total number (with multiplicity)
 *            of prime factors of k. The terms are listed in increasing order
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a sieve up to a fixed limit, collects all
   primes that satisfy the condition, and sorts them once.  Subsequent
   calls to a(idx_t) are served from the sorted table.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Upper bound for k (and therefore for p).  A limit of 2 000 000
   is sufficient to generate more than 10 000 terms in the sequence. */
#define LIMIT      2000000U
/* Capacity of the result table – ample for >10 000 terms. */
#define MAX_TERMS  200000U

static uint32_t spf[LIMIT + 1];           /* smallest prime factor for each k */
static uint8_t  omega_arr[LIMIT + 1];     /* Ω(k) – total number of prime factors of k */
static uint8_t  seen[LIMIT + 1];          /* marks if a prime has already been counted */
static seq_t    seq[MAX_TERMS];           /* sorted sequence values */
static size_t   seq_len = 0;

/* Comparator for seq_t values used by qsort. */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* One‑time construction of the sieve, Ω(k) array and the sequence. */
static void build_sequence(void)
{
    static bool built = false;
    if (built) return;
    built = true;

    /* 1. Initialise spf array. */
    for (uint32_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
    spf[1] = 1;  /* special marker; not prime */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* 2. Compute Ω(k) for all k. */
    omega_arr[0] = 0;
    omega_arr[1] = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        omega_arr[i] = omega_arr[i / spf[i]] + 1;

    /* 3. Collect primes p = k + Ω(k). */
    for (uint32_t i = 0; i <= LIMIT; ++i) seen[i] = 0;
    seq_len = 0;
    for (uint32_t k = 1; k <= LIMIT; ++k) {
        uint32_t p = k + omega_arr[k];
        if (p > 1 && p <= LIMIT && spf[p] == p) {   /* p is prime */
            if (!seen[p]) {
                seen[p] = 1;
                if (seq_len < MAX_TERMS)
                    seq[seq_len++] = (seq_t)p;
            }
        }
    }

    /* 4. Sort the collected primes in ascending order. */
    qsort(seq, seq_len, sizeof(seq[0]), seq_cmp);
}

/* Public function.  n is 1‑based: a(1) = 3, a(2) = 11, … */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 – return 0 as safe value */
    build_sequence();              /* ensure precomputation has happened once */
    size_t idx = (size_t)n - 1;
    if (idx >= seq_len) return 0;  /* out of range – unlikely for n ≤ 10000 */
    return seq[idx];
}
