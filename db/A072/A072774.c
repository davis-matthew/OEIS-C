/*
 * A072774  Powers of squarefree numbers.
 * https://oeis.org/A072774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A072774:  Powers of square‑free numbers
 *  a(n) = m for the nth integer m whose prime factorisation
 *  exponents are all equal.  (i.e., m = (p1 p2 … pk)^e with
 *  p1,…,pk distinct primes.)
 *
 *  The implementation generates the sequence lazily: it
 *  tests successive integers for the “equal‑exponent” property,
 *  keeps the results in a dynamic array, and returns the nth
 *  stored value.  The test uses a simple incremental sieve
 *  to obtain primes up to sqrt(current‑value).
 *
 *  All calculations are performed with 64‑bit unsigned integers.
 *  This is safe up to the thousands of terms requested; for
 *  the initial terms a(n) < 200 000, far below UINT64_MAX.
 *  The result is converted to seq_t only when returned, so
 *  the function works with the generic `seq_t` type used by the
 *  build system (integer or arbitrary‑precision).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *terms      = NULL;   /* stored a(n) values (uint64_t)   */
static size_t    terms_len  = 0;      /* number of values stored          */
static size_t    terms_cap  = 0;      /* allocated capacity of terms      */
static uint64_t  next_check = 1;      /* candidate number to test next    */

static uint32_t *primes      = NULL;  /* list of primes                   */
static size_t    primes_len  = 0;     /* number of primes stored          */
static size_t    primes_cap  = 0;     /* capacity of primes array         */
static uint64_t  last_prime  = 1;     /* highest integer already tested   */

/* ------------------------------------------------------------------ */
/*  Dynamic array helpers                                              */
/* ------------------------------------------------------------------ */
static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 128;
        terms = (uint64_t*)realloc(NULL, terms_cap * sizeof(uint64_t));
    } else if (terms_len == terms_cap) {
        terms_cap *= 2;
        terms = (uint64_t*)realloc(terms, terms_cap * sizeof(uint64_t));
    }
}

/* ------------------------------------------------------------------ */
/*  Prime sieve helpers                                                */
/* ------------------------------------------------------------------ */
static void grow_primes(void)
{
    if (primes_cap == 0) {
        primes_cap = 128;
        primes = (uint32_t*)realloc(NULL, primes_cap * sizeof(uint32_t));
    } else if (primes_len == primes_cap) {
        primes_cap *= 2;
        primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
    }
}

/*  Ensure all primes up to `limit` are stored in `primes`.  The
 *  routine grows the primes list only as needed.  `limit` fits
 *  comfortably inside a 64‑bit unsigned integer.                */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= last_prime) return;

    for (uint64_t x = last_prime + 1; x <= limit; ++x) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > x) break;
            if (x % p == 0) { prime = false; break; }
        }
        if (prime) {
            grow_primes();
            primes[primes_len++] = (uint32_t)x;
        }
    }
    last_prime = limit;
}

/* ------------------------------------------------------------------ */
/*  Test whether m is a power of a square‑free integer.              */
/*  m is assumed to be <= 2^64-1.  The function checks that all
 *  exponents in the prime factorisation of m are equal.            */
static bool is_squarefree_power(uint64_t m)
{
    if (m == 1) return true;            /* 1 = (1)^anything */

    uint64_t val = m;
    int first_exp = -1;

    /* factorise val using the primes list; keep expanding the
     * list up to sqrt(val).                                       */
    while ((uint64_t)primes[primes_len-1] * primes[primes_len-1] <= val)
        ensure_primes_upto(primes[primes_len-1] * primes[primes_len-1]);

    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > val) break;
        if (val % p == 0) {
            int exp = 0;
            while (val % p == 0) {
                val /= p;
                ++exp;
            }
            if (first_exp == -1)
                first_exp = exp;
            else if (exp != first_exp)
                return false;
        }
    }

    /* a remaining factor > 1 is prime with exponent 1 */
    if (val > 1) {
        if (first_exp == -1)
            first_exp = 1;
        else if (first_exp != 1)
            return false;
    }

    return true;
}

/* ------------------------------------------------------------------ */
/*  The required OEIS sequence function                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1, safety guard */

    /* initialise with first prime (2) if primes list empty */
    if (primes_len == 0) {
        grow_primes();
        primes[primes_len++] = 2;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        if (is_squarefree_power(next_check)) {
            grow_terms();
            terms[terms_len++] = next_check;
        }
        ++next_check;
    }

    return (seq_t)terms[n - 1];
}
