/*
 * A174907  Write the prime factorization of the factorial numbers
 *          (p1^q1*p2^q2*...*pn^qn) where p1,p2... are the primes and q1,q2...
 *          are the exponents. The exponents that change with each successive
 *          factorial change to the numbers in the sequence.
 * https://oeis.org/A174907
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the exponent that changes when expressing n! in prime factorisation,
 *  listed in order of increasing n and, for each n, in ascending order of the
 *  prime factors.  Each time a prime's exponent increases, the new exponent
 *  is appended to the sequence.  Examples:
 *     n  a(n)
 *     2   1
 *     3   1
 *     4   3
 *     5   1
 *     6   4
 *     7   2
 *     8   1
 *     9   7
 *    10   4
 *    …                                                    
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const idx_t PRIME_LIMIT = 1000000;   /* maximum prime needed */


/* --------------------------------------------------------------------- */
/*  One‑time initialisation of the prime list, exponent table and cache   */
/* --------------------------------------------------------------------- */

static bool            initialized      = false;
static int            *prime_exponent  = NULL;    /* exponent of each prime */
static int            *primes          = NULL;    /* list of primes */
static size_t          primes_cnt       = 0;       /* number of primes */
static seq_t          *term_cache      = NULL;    /* flattened sequence */
static size_t          term_cap         = 0;       /* storage capacity */
static size_t          term_len         = 0;       /* terms stored */
static idx_t           next_factor      = 2;       /* next factorial to process */

static void init_primes(void)
{
    unsigned char *is_prime = (unsigned char *)malloc((PRIME_LIMIT + 1) * sizeof(unsigned char));
    if (!is_prime) abort();
    for (idx_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    primes = (int *)malloc((PRIME_LIMIT + 1) * sizeof(int));
    if (!primes) abort();
    primes_cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (int)i;

    free(is_prime);
}

static void init_data(void)
{
    if (initialized) return;
    initialized = true;

    prime_exponent = (int *)calloc(PRIME_LIMIT + 1, sizeof(int));
    if (!prime_exponent) abort();

    init_primes();

    term_cap   = 1024;
    term_cache = (seq_t *)malloc(term_cap * sizeof(seq_t));
    if (!term_cache) abort();

    term_len   = 0;
    next_factor = 2;
}

static void ensure_capacity(size_t needed)
{
    if (needed < term_cap) return;
    size_t new_cap = term_cap;
    while (needed >= new_cap) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    term_cache = new_ptr;
    term_cap   = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Public interface                                                    */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values for n<2 are undefined */

    init_data();                  /* initialise once */

    size_t target_index = n - 2;  /* zero‑based index in the cache */

    while (term_len <= target_index) {
        /* factorize next_factor */
        idx_t m = next_factor;
        idx_t t = m;

        for (size_t pi = 0; pi < primes_cnt && (idx_t)primes[pi] * primes[pi] <= t; ++pi) {
            int p = primes[pi];
            if (t % p == 0) {
                int cnt = 0;
                while (t % p == 0) {
                    t /= p;
                    ++cnt;
                }
                prime_exponent[p] += cnt;
                ensure_capacity(term_len + 1);
                term_cache[term_len++] = (seq_t)prime_exponent[p];
            }
        }

        if (t > 1) {              /* remaining prime factor */
            int p = (int)t;
            prime_exponent[p] += 1;
            ensure_capacity(term_len + 1);
            term_cache[term_len++] = (seq_t)prime_exponent[p];
        }

        ++next_factor;
    }

    return term_cache[target_index];
}
