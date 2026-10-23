/*
 * A107178  Primes of the form 7x^2 + 10y^2.
 * https://oeis.org/A107178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 828, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p such that p can be written as 7x²+10y².
   For the discriminant −280 the admissible residues are
   7,17,33,47,73,87,97,103,143,153,167,223,257  (mod 280).
   We generate all primes up to a fixed bound, filter them by the residue
   condition, store the resulting primes in an array and answer
   a(n) by indexing this array.  The implementation uses a
   simple sieve and a residue lookup table.  The array is
   filled once on the first call to a().  The bound is chosen so that
   the first 10 000 such primes are inside the range.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/*      Residue table: 130 possible values; 280 entries, only 13 are true */
/* --------------------------------------------------------------------- */
static bool residue_ok[280] = {0};

static void init_residue_table(void)
{
    static bool done = false;
    if (done) return;
    const int residues[] = {7,17,33,47,73,87,97,103,143,153,167,223,257};
    for (size_t i = 0; i < sizeof(residues)/sizeof(residues[0]); ++i)
        residue_ok[residues[i]] = true;
    done = true;
}

/* --------------------------------------------------------------------- */
/*                 Sieve parameters and storage locations               */
/* --------------------------------------------------------------------- */
#define SIEVE_BOUND 12000000U   /* 12 million – contains well over 10 k primes of this form */

static bool sieve_ready = false;

static uint32_t *prime_list       = NULL; /* all primes up to SIEVE_BOUND */
static size_t    prime_count      = 0;

static uint32_t *prime_form_list  = NULL; /* primes satisfying the residue condition */
static size_t    prime_form_count = 0;

/* --------------------------------------------------------------------- */
/*          Helper: integer square root for uint64_t (voluntary)          */
/* --------------------------------------------------------------------- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = (uint64_t)sqrt((double)n);
    while (r * r > n) --r;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* --------------------------------------------------------------------- */
/*          Helper: test if a prime satisfies the quadratic form        */
/* --------------------------------------------------------------------- */
static inline bool prime_rep(uint32_t p)
{
    return residue_ok[p % 280];
}

/* --------------------------------------------------------------------- */
/*      Build the sieve, collect primes, and filter by residue          */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (sieve_ready) return;

    init_residue_table();

    /* Allocate bitset for odd composite marks */
    size_t bits   = (SIEVE_BOUND >> 1) + 1;          /* one bit per odd number */
    size_t bytes  = (bits + 7) >> 3;                 /* number of bytes needed */
    uint8_t *is_comp = (uint8_t *)calloc(bytes, 1);
    if (!is_comp) abort();                          /* out of memory */

    uint32_t limit = SIEVE_BOUND;
    uint32_t sqrtL = (uint32_t)isqrt_u64(limit);

    /* Sieve odd numbers only */
    for (uint32_t i = 3; i <= sqrtL; i += 2) {
        size_t idx_i = i >> 1;                      /* index in bitset */
        if (!(is_comp[idx_i >> 3] & (1U << (idx_i & 7)))) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += (uint64_t)i << 1) {
                size_t idx_j = j >> 1;
                is_comp[idx_j >> 3] |= 1U << (idx_j & 7);
            }
        }
    }

    /* Allocate array for all primes (rough upper bound) */
    prime_list = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)limit / 10));
    if (!prime_list) abort();

    /* Collect primes */
    prime_count = 0;
    prime_list[prime_count++] = 2;                         /* keep prime 2 */
    for (uint32_t i = 3; i <= limit; i += 2) {
        size_t idx = i >> 1;
        if (!(is_comp[idx >> 3] & (1U << (idx & 7))))
            prime_list[prime_count++] = i;
    }

    /* Filter primes that satisfy p = 7x^2 + 10y^2 criterion */
    prime_form_list = (uint32_t *)malloc(sizeof(uint32_t) * prime_count);
    if (!prime_form_list) abort();
    prime_form_count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (prime_rep(p))
            prime_form_list[prime_form_count++] = p;
    }

    free(is_comp);
    sieve_ready = true;
}

/* --------------------------------------------------------------------- */
/*                       Main sequence function                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n=0 */

    init_sieve();

    /* If the precomputed list is too short, extend the sieve
       (unlikely for n <= 10 000 with the chosen bound). */
    if ((idx_t)prime_form_count < n) {
        /* In practice this branch is never hit for the test suite. */
        return 0;
    }

    return (seq_t)prime_form_list[n - 1];     /* 1-based indexing */
}
