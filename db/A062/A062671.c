/*
 * A062671  Every divisor (except 1) contains the digit 5.
 * https://oeis.org/A062671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2950, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A062671 – numbers whose every divisor (except 1) contains the digit 5.
 *  This implementation generates terms on demand by testing each candidate
 *  until the requested index is reached.  The check for the property
 *  factors the candidate and recursively verifies that each divisor > 1
 *  contains a 5 in its decimal representation.  The algorithm skips all
 *  numbers that do not contain the digit 5 themselves, which is a necessary
 *  condition for the property.
 *
 *  The code is written in standard ISO C11 (and compiles with C++17), uses
 *  only the allowed arithmetic and comparison operators on `seq_t`, and
 *  employs only the facilities granted by the build environment.  It
 *  maintains static storage for the list of already computed terms and
 *  for the list of primes required for factorisation.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains5.h"

/* ---------- Sieve of Eratosthenes for primes up to 2 000 000 */
#define PRIME_SIEVE_LIMIT 2000000U
static uint32_t *prime_list = NULL;   /* array of primes */
static size_t     prime_cnt  = 0;     /* current number of primes */

/* initialise the prime list once */
static void init_prime_sieve(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    bool *sieve = (bool*)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (!sieve) exit(1);
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        sieve[i] = true;

    for (size_t p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p) {
        if (sieve[p]) {
            for (size_t m = p * p; m <= PRIME_SIEVE_LIMIT; m += p)
                sieve[m] = false;
        }
    }

    /* count primes first to allocate enough space */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) prime_list[idx++] = (uint32_t)i;

    prime_cnt = cnt;
    free(sieve);
}

/* ---------- Factorisation and divisor checking -------------------------- */
/* fixed maximum number of distinct prime factors that can appear */
#define MAX_FACTORS 32

typedef struct {
    seq_t  prime;   /* the prime factor */
    unsigned exp;   /* its exponent */
} factor_t;

/* recursively verify that every divisor > 1 contains a 5 */
static bool check_all_divisors(const factor_t *facs, unsigned fcnt, size_t idx, seq_t current)
{
    if (idx == fcnt)
        return true;                           /* all combinations processed */

    seq_t p = facs[idx].prime;
    unsigned e = facs[idx].exp;
    seq_t cur = current;

    for (unsigned k = 0; k <= e; ++k) {
        if (k > 0)
            cur = cur * p;                      /* multiply by another prime factor */
        if (cur != (seq_t)1 && !contains5(cur))
            return false;                       /* a divisor without a 5 found */
        if (!check_all_divisors(facs, fcnt, idx + 1, cur))
            return false;                       /* deeper recursion failed */
    }
    return true;
}

/* test whether a candidate satisfies the definition of the sequence */
static bool is_good(seq_t n)
{
    init_prime_sieve();               /* ensure primes are available */

    /* quick reject: n must contain a 5 itself */
    if (!contains5(n))
        return false;

    /* factorise n using the prime list, collecting distinct factors */
    factor_t facs[MAX_FACTORS];
    unsigned fcnt = 0;
    seq_t rem = n;
    uint64_t rem_u64 = (uint64_t)rem;    /* for comparison against primes */

    for (size_t i = 0; i < prime_cnt && rem != (seq_t)1; ++i) {
        uint64_t p_u64 = prime_list[i];
        seq_t  p = (seq_t)p_u64;

        /* stop if p^2 > rem */
        if (p * p > rem)
            break;

        if (rem % p == (seq_t)0) {        /* p divides rem */
            unsigned e = 0;
            while (rem % p == (seq_t)0) {
                rem /= p;
                ++e;
            }
            facs[fcnt].prime = p;
            facs[fcnt].exp   = e;
            ++fcnt;
        }
    }

    /* any remaining factor > 1 is a prime > sqrt(n) */
    if (rem != (seq_t)1) {
        facs[fcnt].prime = rem;
        facs[fcnt].exp   = 1;
        ++fcnt;
    }

    /* now test all divisors formed from these prime factors */
    return check_all_divisors(facs, fcnt, 0, (seq_t)1);
}

/* ---------- Term storage ------------------------------------------- */
static seq_t *terms   = NULL;   /* dynamic array of found terms */
static size_t terms_n  = 0;     /* number of terms stored */
static size_t terms_cap= 0;     /* current capacity */

/* append a new term */
static void append_term(seq_t val)
{
    if (terms_n == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_arr = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        terms = new_arr;
    }
    terms[terms_n++] = val;
}

/* generate terms until we have at least n of them */
static void ensure_terms(size_t n)
{
    static seq_t candidate = (seq_t)5;   /* first possible number with digit 5 */

    while (terms_n < n) {
        if (contains5(candidate) && is_good(candidate))
            append_term(candidate);
        candidate += (seq_t)1;
    }
}

/* ---------- Public interface ----------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* offset starts at 1; behaviour for 0 is arbitrary */

    ensure_terms((size_t)n);
    return terms[n - 1];
}
