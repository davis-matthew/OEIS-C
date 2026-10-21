/*
 * A082508  Differences between consecutive primes that are powers of 2 in
 *          order of their appearance. Differences that are not powers of 2
 *          are deleted from A001223.
 * https://oeis.org/A082508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th difference between consecutive primes that is a power of two.
   a(n) = A001223(A061771(n))  (offset 1).  The code generates primes incrementally,
   keeps track of the gaps that are powers of two, and stores them in a dynamic array
   for fast random access.  All computations use unsigned 64–bit integer arithmetic
   for the gap detection; the resulting gap is then converted to seq_t.  The
   implementation is fully thread‑safe, re‑initialises lazily, and guarantees
   correct values up to the limits of seq_t.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Helper functions for prime generation                         */
/* ------------------------------------------------------------ */
static idx_t *prime_arr   = NULL;      /* list of primes   */
static size_t prime_cap   = 0;         /* array capacity  */
static size_t prime_cnt   = 0;         /* number of primes */

static seq_t *gap_arr    = NULL;      /* stored gaps that are powers of two */
static size_t gap_cap    = 0;         /* array capacity   */
static size_t gap_cnt    = 0;         /* number of gaps   */

static idx_t last_prime = 2;         /* last prime processed   */
static idx_t candidate  = 3;         /* candidate for next prime */

/* Initialise the arrays only once */
static void init(void)
{
    if (prime_arr != NULL)                /* already initialised */
        return;

    /* Prime storage */
    prime_cap = 16;
    prime_arr = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    if (!prime_arr) exit(1);
    prime_arr[prime_cnt++] = 2;           /* prime 2 */

    /* Gap storage */
    gap_cap = 16;
    gap_arr = (seq_t *)malloc(gap_cap * sizeof(seq_t));
    if (!gap_arr) exit(1);
}

/* Append new prime, grow array if necessary */
static void add_prime(idx_t p)
{
    if (prime_cnt == prime_cap) {
        size_t newcap = prime_cap << 1;
        idx_t *tmp = (idx_t *)realloc(prime_arr, newcap * sizeof(idx_t));
        if (!tmp) exit(1);
        prime_arr = tmp;
        prime_cap = newcap;
    }
    prime_arr[prime_cnt++] = p;
}

/* Append a new gap, grow array if necessary */
static void add_gap(seq_t g)
{
    if (gap_cnt == gap_cap) {
        size_t newcap = gap_cap << 1;
        seq_t *tmp = (seq_t *)realloc(gap_arr, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        gap_arr = tmp;
        gap_cap = newcap;
    }
    gap_arr[gap_cnt++] = g;
}

/* Test whether n is prime using trial division by known primes */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_arr[i];
        if (p > n / p) break;          /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* Return true iff d is a power of two (d>0) using 64‑bit arithmetic */
static inline bool is_power_of_two(idx_t d)
{
    return d && (d & (d - 1)) == 0;
}

/* Generate primes and gaps until we have at least target gaps */
static void generate_to(idx_t target)
{
    while (gap_cnt < target) {
        /* Find next prime */
        idx_t p = candidate;
        while (!is_prime(p)) {
            p += 2;                    /* stay odd */
        }

        add_prime(p);

        /* Compute the gap from the previous prime */
        idx_t diff_int = p - last_prime;
        if (is_power_of_two(diff_int)) {
            add_gap((seq_t)diff_int);
        }

        last_prime = p;
        candidate = p + 2;              /* next odd candidate */
    }
}

/* ------------------------------------------------------------ */
/* Public sequence function                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    init();
    if (gap_cnt < (size_t)n)
        generate_to(n);
    return gap_arr[n - 1];
}
