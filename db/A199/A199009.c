/*
 * A199009  Composite numbers whose prime factors have an equal number of
 *          digits in decimal representation.
 * https://oeis.org/A199009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 2836, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A199009: composite numbers whose prime factors all have equal number of decimal digits.
   This implementation builds the sequence by incremental trial‑division factorization.
   Primes are generated lazily and stored in a dynamic array; the sequence cache is
   also dynamic.  The algorithm is O(n·sqrt(m)) where m is the n‑th term, which is
   easily fast enough for n ≈ 10 000.  The code is valid C11 and C++17. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Helper: integer square root (binary search via Newton iteration) --- */
static uint64_t uint64_sqrt(uint64_t n)
{
    if (n <= 1) return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        unsigned __int128 sum = (unsigned __int128)x + (n / x);
        y = (uint64_t)(sum >> 1);
    }
    return x;
}

/* --- Prime list management ---------------------------------------------- */
static uint64_t *primes = NULL;          /* dynamic array of primes */
static size_t    primes_len = 0;         /* number of primes stored   */
static size_t    primes_cap = 0;         /* allocated capacity of primes array */
static uint64_t  next_prime_candidate = 3;   /* odd number to test next */

/* grow the primes array (double its capacity) */
static void primes_grow(void)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    uint64_t *new_ptr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!new_ptr) return;    /* allocation failed – silently abort (sequence may be incomplete) */
    primes = new_ptr;
    primes_cap = new_cap;
}

/* append a prime to the list */
static void primes_add(uint64_t p)
{
    if (primes_len == primes_cap) primes_grow();
    primes[primes_len++] = p;
}

/* generate all primes up to (and including) limit, using trial division against known primes */
static void primes_up_to(uint64_t limit)
{
    while (next_prime_candidate <= limit) {
        uint64_t p = next_prime_candidate;
        bool isP = true;
        /* test divisibility by primes <= sqrt(p) */
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t q = primes[i];
            if (q * q > p) break;
            if (p % q == 0) { isP = false; break; }
        }
        if (isP) primes_add(p);
        next_prime_candidate += 2;   /* skip even numbers */
    }
}

/* --- Sequence cache management ------------------------------------------- */
static seq_t *seq_cache = NULL;   /* dynamic array of sequence values */
static size_t  seq_len   = 0;     /* number of values stored */
static size_t  seq_cap   = 0;     /* allocated capacity of cache   */

/* grow the sequence cache (double its capacity) */
static void seq_cache_grow(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(*seq_cache));
    if (!new_ptr) return;
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* append a value to the sequence cache */
static void seq_cache_append(seq_t val)
{
    if (seq_len == seq_cap) seq_cache_grow();
    seq_cache[seq_len++] = val;
}

/* --- Core test: does N have prime factors all with the same decimal digit count? --- */
static unsigned int digit_count(uint64_t x)
{
    unsigned int d = 0;
    do { ++d; x /= 10; } while (x > 0);
    return d;
}

static bool same_digit_factors(uint64_t N)
{
    if (N < 4) return false;            /* 1,2,3 are not composite */
    uint64_t temp = N;
    bool composite = false;             /* at least one non‑trivial factor? */
    int first_digits = -1;

    /* factorization using prime list */
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            composite = true;
            int d = digit_count(p);
            if (first_digits == -1) first_digits = d;
            else if (d != first_digits) return false;
            while (temp % p == 0) temp /= p;
        }
    }

    /* primality test: if no divisor found, N itself is prime */
    if (!composite && temp == N) return false;

    /* remaining prime factor > sqrt(N) */
    if (temp > 1) {
        int d = digit_count(temp);
        if (first_digits == -1) first_digits = d;
        else if (d != first_digits) return false;
    }

    return true;
}

/* --- Main driver: compute the sequence up to index n ------------------ */
static uint64_t next_candidate = 2;   /* next integer to test */

/* ensure that a(n) has been generated and cached */
static void compute_up_to(idx_t n)
{
    if ((size_t)n <= seq_len) return;    /* already computed */

    /* initialise primes list with 2 at first call */
    if (primes_len == 0) {
        primes = (uint64_t *)malloc(16 * sizeof(uint64_t));
        if (!primes) return;
        primes_cap = 16;
        primes_add(2);
    }

    while (seq_len < (size_t)n) {
        uint64_t limit = uint64_sqrt(next_candidate);
        primes_up_to(limit);

        if (same_digit_factors(next_candidate))
            seq_cache_append((seq_t)next_candidate);

        ++next_candidate;
    }
}

/* ------------------------------------------------------------------------ */
/* The required function with external linkage. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* behaviour for n=0 is irrelevant */
    compute_up_to(n);
    return seq_cache[n - 1];
}
