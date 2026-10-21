/*
 * A088007  Numbers n such that abs(sigma(n) - 2n) <= sqrt(n).
 * https://oeis.org/A088007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m with |sigma(m) - 2m| <=
 *            sqrt(m)
 * Range:     u8 n <= 19, u16 n <= 1559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sigma is the sum of all positive divisors of m)                            */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/* Integer square root (floor) for idx_t                                       */
/* -------------------------------------------------------------------------- */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 <= n) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow when mid == 0 */
            high = mid - 1;
        }
    }
    return res;
}

/* -------------------------------------------------------------------------- */
/* Prime list generation (simple Eratosthenes)                                 */
/* -------------------------------------------------------------------------- */
static bool *prime_is_comp = NULL;        /* 0 -> prime, 1 -> composite   */
static size_t prime_is_comp_limit = 0;    /* highest index that is generated */
static idx_t *prime_list = NULL;         /* array of primes              */
static size_t prime_list_size = 0;       /* number of primes found       */
static size_t prime_list_cap = 0;        /* allocated capacity for primes */

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_is_comp_limit)
        return;

    /* Grow the is_composite array to at least 'limit'  */
    size_t new_limit = prime_is_comp_limit ? prime_is_comp_limit : 2;
    while (new_limit < (size_t)limit) new_limit <<= 1;

    bool *tmp = (bool *)realloc(prime_is_comp, new_limit + 1);
    if (!tmp) return;                     /* allocation failed – abort silently */

    /* initialise new entries to composite = 0 (prime) */
    for (size_t i = prime_is_comp_limit + 1; i <= new_limit; ++i)
        tmp[i] = 0;
    prime_is_comp = tmp;
    prime_is_comp_limit = new_limit;

    /* Recompute all primes up to new_limit */
    prime_list_size = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (!prime_is_comp[i]) {
            /* i is prime – add to list */
            if (prime_list_size >= prime_list_cap) {
                prime_list_cap = prime_list_cap ? prime_list_cap * 2 : 16;
                idx_t *tmp2 = (idx_t *)realloc(prime_list, prime_list_cap * sizeof(idx_t));
                if (!tmp2) return;       /* allocation failed – abort silently */
                prime_list = tmp2;
            }
            prime_list[prime_list_size++] = (idx_t)i;

            /* Mark multiples of i */
            if (i * i <= new_limit) {
                for (size_t j = i * i; j <= new_limit; j += i)
                    prime_is_comp[j] = 1;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Compute sigma(n) using the prime list                                       */
/* -------------------------------------------------------------------------- */
static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;
    for (size_t pi = 0; pi < prime_list_size; ++pi) {
        idx_t p = prime_list[pi];
        if ((idx_t)p * (idx_t)p > temp) break;

        if (temp % p == 0) {
            seq_t p_pow = 1;
            seq_t sum = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= (seq_t)p;
                sum += p_pow;
            }
            result *= sum;
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        result *= (seq_t)(1 + temp);
    }
    return result;
}

/* -------------------------------------------------------------------------- */
/* Storage for the discovered terms                                           */
/* -------------------------------------------------------------------------- */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap = 0;
static idx_t max_checked = 0;   /* largest integer examined so far */

/* Ensure capacity for at least one more term */
static void ensure_terms_capacity(void)
{
    if (terms_cap > terms_size) return;
    terms_cap = terms_cap ? terms_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    if (!tmp) return;            /* allocation failed – abort silently */
    terms = tmp;
}

/* -------------------------------------------------------------------------- */
/* Main sequence function                                                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out of range */

    /* Produce terms until we have at least n of them */
    while (terms_size < (size_t)n) {
        idx_t candidate = max_checked + 1;

        /* Compute sigma up to the candidate */
        /* Ensure we have primes up to sqrt(candidate) */
        idx_t sieve_limit = int_sqrt(candidate) + 1;
        ensure_primes(sieve_limit);

        seq_t sigma_val = sigma_of(candidate);

        /* Check the inequality |sigma(n) - 2n| <= sqrt(n) */
        seq_t two_n = (seq_t)candidate * 2;
        seq_t diff = (sigma_val >= two_n) ? (sigma_val - two_n) : (two_n - sigma_val);
        idx_t sqrt_candidate = int_sqrt(candidate);

        if (diff <= (seq_t)sqrt_candidate) {
            ensure_terms_capacity();
            terms[terms_size++] = (seq_t)candidate;
        }

        max_checked = candidate;
    }

    return terms[n - 1];
}
