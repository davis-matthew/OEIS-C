/*
 * A115231  Primes p which cannot be written in the form 2^i + q^j where i >=
 *          0, j >= 1, q = odd prime.
 * https://oeis.org/A115231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 822, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A115231: primes p that cannot be expressed as 2^i + q^j
   with i ≥ 0, j ≥ 1, and q an odd prime.
   This routine returns the nth such prime (n ≥ 1).
   The implementation precomputes all primes up to a fixed limit,
   marks all odd prime powers, and filters the primes that are not
   representable.  The list is kept in a static array for fast
   repeated access.
*/

#include <stdlib.h>
#include <stdbool.h>

/* -------------------- static data -------------------- */
static bool   *is_prime        = NULL;   /* sieve of Eratosthenes */
static bool   *is_prime_power  = NULL;   /* flag for odd prime powers */
static idx_t  *prime_list      = NULL;   /* primes up to SIEVE_LIMIT */
static size_t  prime_count     = 0;      /* number of primes */

static seq_t  *seq_terms       = NULL;   /* sequence terms (non‑representable primes) */
static size_t  seq_terms_size  = 0;      /* amount stored */
static size_t  seq_terms_cap   = 0;      /* capacity allocated */

static bool    initialized     = false;

/* upper bound for the sieve – sufficient to reach >10 000 terms
   (~5 million gives ~46 000 sequence terms) */
#define SIEVE_LIMIT 5000000ULL

/* -------------------- helper -------------------------------- */
static void init(void)
{
    if (initialized) return;

    /* ---- sieve of Eratosthenes ---- */
    is_prime = (bool *)calloc((size_t)SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_prime) abort();

    for (idx_t i = 2; (idx_t)(i * i) <= SIEVE_LIMIT; ++i)
        if (!is_prime[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = true;

    /* collect primes */
    size_t primes_cap = 2048;
    prime_list = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!prime_list) abort();

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_prime[i]) {
            if (prime_count == primes_cap) {
                primes_cap <<= 1;
                prime_list = (idx_t *)realloc(prime_list,
                                              primes_cap * sizeof(idx_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = i;
        }

    /* ---- mark odd prime powers ---- */
    is_prime_power = (bool *)calloc((size_t)SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_prime_power) abort();

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t q = prime_list[pi];
        if (q == 2) continue;                 /* odd primes only */

        idx_t pp = q;
        while (pp <= SIEVE_LIMIT) {
            is_prime_power[pp] = true;
            if (pp > SIEVE_LIMIT / q) break;
            pp *= q;
        }
    }

    /* ---- build the sequence terms ---- */
    seq_terms_cap = 16384;
    seq_terms = (seq_t *)malloc(seq_terms_cap * sizeof(seq_t));
    if (!seq_terms) abort();

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = prime_list[pi];
        bool representable = false;

        /* i = 0 gives 2^0 = 1, so p - 1 must be an odd prime power;
           for p > 2 this cannot hold (p-1 even), so skip i=0. */
        for (idx_t i = 1;
             (idx_t)((1ULL << i)) < p; ++i) {
            idx_t pow2 = (idx_t)(1ULL << i);
            idx_t r = p - pow2;                 /* r is odd for odd p, i ≥ 1 */
            if (r <= SIEVE_LIMIT && is_prime_power[r]) {
                representable = true;
                break;
            }
        }

        if (!representable) {
            if (seq_terms_size == seq_terms_cap) {
                seq_terms_cap <<= 1;
                seq_terms = (seq_t *)realloc(seq_terms,
                                             seq_terms_cap * sizeof(seq_t));
                if (!seq_terms) abort();
            }
            seq_terms[seq_terms_size++] = (seq_t)p;
        }
    }

    /* ---- free temporary arrays ---- */
    free(is_prime);
    free(prime_list);
    free(is_prime_power);

    initialized = true;
}

/* -------------------- public function -------------------- */
seq_t a(idx_t n)
{
    init();

    /* the sequence is defined for n ≥ 1; out‑of‑range requests
       return 0, which callers can interpret as “not found”. */
    if (n == 0 || n > (idx_t)seq_terms_size)
        return 0;

    return seq_terms[(size_t)(n - 1)];
}
