/*
 * A101434  Composite numbers which are the product of non-twin primes.
 * https://oeis.org/A101434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Composite numbers which are the product of non‑twin primes. */

/*  The sequence consists of all composite integers whose prime factors are
 *  all non–twin primes (primes that do not belong to a twin pair).
 *  For example, 4 = 2×2, 46 = 2×23 are in the sequence; 9 = 3×3 is not
 *  because 3 is a twin prime (3 and 5).  The function returns a(n) for
 *  1 ≤ n.  The implementation uses a sieve up to MAX_SIEVE (2 000 000)
 *  which is sufficient for n up to about 10 000.  It memoises the
 *  sequence so that calls with any order of indices are fast.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIEVE 2000000

/*------------------------------------------------------------*/
/*  Sieve of Eratosthenes: primes up to MAX_SIEVE             */
static unsigned char *is_prime = NULL;   /* 1 = prime, 0 = composite */
static idx_t *prime_list = NULL;        /* list of primes */
static size_t prime_cnt = 0;

/*  Twin‑prime flag: 1 if prime p is part of any twin pair (p‑2 or p+2 is prime) */
static unsigned char *is_twin_prime = NULL;

static void init_sieve(void)
{
    if (is_prime) return;

    size_t limit = MAX_SIEVE;
    is_prime = (unsigned char *)malloc(limit + 1);
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Allocate space for primes (≤ limit/ln(limit) ), safe size 200 000 */
    prime_list = (idx_t *)malloc((limit / 10) * sizeof(idx_t));
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = i;
}

/*------------------------------------------------------------*/
/*  Precompute twin‑prime flags                                */
static void init_twin_flag(void)
{
    if (is_twin_prime) return;
    init_sieve();

    size_t limit = MAX_SIEVE;
    is_twin_prime = (unsigned char *)malloc(limit + 1);
    memset(is_twin_prime, 0, limit + 1);

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_prime[i]) continue;
        bool twin = false;
        if (i >= 2 && is_prime[i - 2]) twin = true;
        if (i + 2 <= limit && is_prime[i + 2]) twin = true;
        is_twin_prime[i] = twin ? 1 : 0;
    }
}

/*------------------------------------------------------------*/
/*  Memoised sequence storage                                 */
static seq_t *terms = NULL;          /* array of sequence terms */
static size_t terms_used = 0;        /* how many terms stored */
static size_t terms_cap = 0;         /* current capacity of array */
static uint64_t next_candidate = 4;  /* next integer to test */

static void ensure_capacity(void)
{
    if (terms_used < terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_arr;
    terms_cap = new_cap;
}

static void add_term(seq_t val)
{
    ensure_capacity();
    terms[terms_used++] = val;
}

/*------------------------------------------------------------*/
/*  Main sequence function                                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    /* Ensure everything is initialised. */
    init_twin_flag();

    /* Generate terms until we have at least n of them. */
    while (terms_used < (size_t)n) {
        uint64_t m = next_candidate++;
        if (m <= MAX_SIEVE && is_prime[m]) continue; /* skip primes */

        bool ok = true;
        uint64_t tmp = m;

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint64_t p = prime_list[pi];
            if (p * p > tmp) break;
            if (tmp % p == 0) {
                if (p <= MAX_SIEVE && is_twin_prime[p]) { ok = false; break; }
                while (tmp % p == 0) tmp /= p;
            }
        }
        if (!ok) continue;

        /* Remaining factor > 1 (may be prime) */
        if (tmp > 1) {
            if (tmp <= MAX_SIEVE && is_twin_prime[tmp]) continue;
            /* If tmp > MAX_SIEVE we would need to test primality; this
               case is not expected for the required limits. */
        }

        /* All prime factors are non‑twin primes.  m is composite. */
        add_term((seq_t)m);
    }

    return terms[n - 1];
}
