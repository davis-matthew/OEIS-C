/*
 * A108604  Squares of the form prime(k)*prime(k+1) + 2*prime(k+1).
 * https://oeis.org/A108604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (greater twin prime of the n-th twin prime pair)^2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 860, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- prime table, grows as needed --------- */
static uint64_t *prime_tbl = NULL;      /* array of known primes (ascending) */
static size_t prime_cnt = 0;            /* number of stored primes    */
static size_t prime_cap = 0;            /* current capacity of table  */

/* --------- twin‑prime table, grows as needed --------- */
static uint64_t *twin_tbl = NULL;       /* array of greater element of each twin pair */
static size_t twin_cnt = 0;             /* number of stored twin primes      */
static size_t twin_cap = 0;             /* current capacity of twin table     */

static uint64_t next_candidate = 5;     /* next odd number to test for twinning */

/* Ensure prime table has space for a new entry.  */
static void prime_tbl_grow(void)
{
    size_t new_cap = (prime_cap == 0) ? 8 : prime_cap << 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
    if (!tmp) abort(); /* memory exhaustion */
    prime_tbl = tmp;
    prime_cap = new_cap;
}

/* Ensure twin table has space for a new entry.  */
static void twin_tbl_grow(void)
{
    size_t new_cap = (twin_cap == 0) ? 8 : twin_cap << 1;
    uint64_t *tmp = (uint64_t *)realloc(twin_tbl, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    twin_tbl = tmp;
    twin_cap = new_cap;
}

/* Trial division primality test using the known prime list.  */
static bool is_prime_test(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p > n / p) break;           /* p * p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* Trial division primality test that also records a newly found prime.  */
static bool is_prime_and_add(uint64_t n)
{
    if (!is_prime_test(n)) return false;

    /* store the new prime */
    if (prime_cnt == prime_cap) prime_tbl_grow();
    prime_tbl[prime_cnt++] = n;
    return true;
}

/* Initialise the prime table with the first two primes.  */
static void init_prime_tbl(void)
{
    if (prime_tbl) return;            /* already initialised */
    prime_tbl = (uint64_t *)malloc(8 * sizeof(uint64_t));
    if (!prime_tbl) abort();
    prime_cap = 8;
    prime_tbl[0] = 2;
    prime_tbl[1] = 3;
    prime_cnt  = 2;
}

/* Find n‑th twin prime (greater element of the pair) on demand.  */
static uint64_t twin_larger_n(idx_t n)
{
    init_prime_tbl();

    /* grow twin table until we have at least n entries */
    while (twin_cnt < (size_t)n) {
        uint64_t p = next_candidate;
        /* search for next twin prime pair */
        while (true) {
            if (is_prime_and_add(p) && is_prime_test(p - 2)) {
                /* p is prime and p-2 is prime – found a twin pair */
                break;
            }
            p += 2; /* skip even numbers */
        }
        /* store the greater twin prime */
        if (twin_cnt == twin_cap) twin_tbl_grow();
        twin_tbl[twin_cnt++] = p;
        next_candidate = p + 2;          /* start searching after this pair */
    }
    return twin_tbl[n - 1];              /* zero‑based index */
}

/* ------------------------------------------------------------------ */
/*  Public API:  seq_t a(idx_t n)  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n=0 – return 0 */
    uint64_t p = twin_larger_n(n);
    return (seq_t)p * (seq_t)p;         /* square of the twin prime */
}
