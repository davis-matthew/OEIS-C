/*
 * A075736  Fibonacci numbers F(k) as k runs through the products of an odd
 *          number of distinct primes A030059 (mu(k)=-1).
 * https://oeis.org/A075736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = Fibonacci(k) where k is the n‑th integer with μ(k) = –1
 * (i.e., a product of an odd number of distinct primes). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* idx_t and seq_t are supplied by the build system.  They are used here
 * directly without redefinition. */

/* ---------- State for sieve of μ‑odd numbers -------------------------------- */

static idx_t sieve_limit = 0;          /* highest number pre‑sieved   */
static int8_t *mu = NULL;              /* Möbius values for [1..limit] */
static unsigned char *is_comp = NULL;  /* composite flag in linear sieve */

static idx_t *primes = NULL;           /* list of primes */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static idx_t *km = NULL;               /* list of μ‑odd numbers         */
static size_t km_cnt = 0;              /* current length of km          */
static size_t km_cap = 0;              /* allocated capacity of km      */

/* --------------------------------------------------------------------- */
/* Resize the primes array to meet the requested capacity.                */
static void ensure_primes_cap(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Resize the km array to meet the requested capacity.                    */
static void ensure_km_cap(size_t needed)
{
    if (needed <= km_cap) return;
    size_t new_cap = km_cap ? km_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(km, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    km = tmp;
    km_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Recompute the Möbius function up to new_limit and rebuild km.          */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    /* Allocate or enlarge arrays for the new range. */
    mu = (int8_t *)realloc(mu, (new_limit + 1) * sizeof(int8_t));
    is_comp = (unsigned char *)realloc(is_comp,
                                       (new_limit + 1) * sizeof(unsigned char));
    if (!mu || !is_comp) abort();

    /* Initialize data structures for the new range. */
    memset(mu, 0, (new_limit + 1) * sizeof(int8_t));
    memset(is_comp, 0, (new_limit + 1) * sizeof(unsigned char));
    mu[1] = 1;                  /* μ(1) = 1 */

    /* Prepare primes list. */
    primes_cnt = 0;
    ensure_primes_cap(new_limit / 10 + 10);

    /* Linear sieve computing μ and gathering primes. */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            mu[i] = -1;          /* prime → μ = –1 */
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > new_limit) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;          /* square factor → μ = 0 */
                break;
            } else {
                mu[v] = -mu[i];      /* toggle sign */
            }
        }
    }

    /* Rebuild km: list of all k with μ(k) = –1. */
    km_cnt = 0;
    ensure_km_cap(new_limit);        /* km will hold at most new_limit numbers */
    for (idx_t i = 1; i <= new_limit; ++i) {
        if (mu[i] == -1) km[km_cnt++] = i;
    }

    sieve_limit = new_limit;
}

/* --------------------------------------------------------------------- */
/* Ensure that km contains at least `need` elements.  Extend sieve if required. */
static void ensure_km_upto(size_t need)
{
    /* Rough upper bound: about 30 % of numbers are μ‑odd.  Use 4×need for safety. */
    idx_t estimate = 4 * need;
    if (estimate < 10) estimate = 10;
    while (km_cnt < need) {
        extend_sieve(estimate);
        estimate <<= 1;   /* grow until sufficient size is reached */
    }
}

/* ---------- Fast doubling Fibonacci ------------------------------------ */

/* Recursive helper that returns F(n) in *f and F(n+1) in *fp. */
static void fib_pair(idx_t n, seq_t *f, seq_t *fp)
{
    if (n == 0) {
        *f  = 0;
        *fp = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);         /* a = F(k), b = F(k+1), k = n/2 */
    seq_t two_b = b + b;              /* 2*F(k+1) */
    seq_t c = a * (two_b - a);        /* F(2k)   = F(k)*(2*F(k+1)-F(k)) */
    seq_t d = a * a + b * b;          /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    if ((n & 1) == 0) {
        *f  = c;
        *fp = d;
    } else {
        *f  = d;
        *fp = c + d;                  /* F(2k+1) and F(2k+2) */
    }
}

static seq_t fib_value(idx_t n)
{
    seq_t f, fp;
    fib_pair(n, &f, &fp);
    return f;
}

/* ---------- Public sequence function ----------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, safe guard */
    ensure_km_upto((size_t)n);           /* ensure km[n-1] exists */

    idx_t k = km[n - 1];                 /* the n‑th μ‑odd number */
    return fib_value(k);                 /* Fibonacci(k) */
}
