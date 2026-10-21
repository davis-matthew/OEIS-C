/*
 * A210480  Number of primes p<n with p-1 and p+1 both practical, and n-p
 *          prime or practical.
 * https://oeis.org/A210480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that p-1 and p+1 are
 *            practical, and n-p is prime or practical
 * Range:     u8 n <= 9316, u16 n <= 9316, u32 n <= 9316, u64 n <= 9316,
 *            u128 n <= 9316, big any n
 * Helpers:   build_primes_upto, build_spf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helpers */
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/is_prime.h"

/* static storage for primes / spf / practical flags */
static uint64_t *primes = NULL;   /* list of primes up to the current limit */
static size_t   prime_cnt = 0;    /* number of primes in the list */

static idx_t *spf = NULL;         /* smallest prime factor array  (size = limit+1) */
static size_t  spf_size = 0;      /* current size of spf array  (limit+1) */

static bool *ispractical = NULL;  /* ispractical[i] = true if i is practical */
static size_t  practical_size = 0;/* current size of ispractical array (limit+1) */

/* ------------------------------------------------------------------ */
/* Utility: compute practicalness for numbers from old_limit+1 to new_limit */
static void compute_practical_array(idx_t new_limit)
{
    /* enlarge the array first */
    bool *new_arr = (bool *)calloc(new_limit + 1, sizeof(bool));
    if (!new_arr) return;           /* out of memory – caller will still return 0 */
    /* copy old data */
    for (size_t i = 0; i < practical_size; ++i)
        new_arr[i] = ispractical[i];
    /* free old array */
    free(ispractical);
    ispractical = new_arr;
    /* now compute practicality for each new number */
    for (idx_t x = practical_size; x <= new_limit; ++x) {
        if (x <= 1) {               /* 1 is practical by definition */
            ispractical[x] = true;
            continue;
        }
        bool ok = true;
        idx_t y = x;
        idx_t sigma_prev = 1;       /* product of sigma of previous prime powers */
        while (y > 1) {
            idx_t p = spf[y];
            idx_t exp = 0;
            while (y % p == 0) {
                y /= p;
                ++exp;
            }
            if (p > sigma_prev + 1) {
                ok = false;
                break;
            }
            /* compute sigma of p^exp: (p^(exp+1)-1)/(p-1) */
            idx_t power = 1;
            for (size_t i = 0; i < exp; ++i)
                power *= p;                /* fits in idx_t for small limits */
            idx_t numerator   = power * p - 1;
            idx_t denominator = p - 1;
            idx_t sigma_pa = numerator / denominator;
            sigma_prev *= sigma_pa;
        }
        ispractical[x] = ok;
    }
    practical_size = new_limit + 1;
}

/* ------------------------------------------------------------------ */
/* Ensure we have spf array up to limit */
static void ensure_spf(idx_t limit)
{
    if (spf_size > limit)
        return;                       /* already large enough */
    free(spf);
    build_spf(limit, &spf);
    spf_size = limit + 1;
}

/* ------------------------------------------------------------------ */
/* Ensure we have primes list up to limit (inclusive) */
static void ensure_primes(idx_t limit)
{
    free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* we need data up to n (for p+1, n-p, etc.) */
    idx_t limit = n;                 /* n itself is needed for practicalness */

    /* compute / extend spf */
    ensure_spf(limit);

    /* compute / extend practicality table */
    if (practical_size <= limit)
        compute_practical_array(limit);

    /* compute / extend primes list up to n-1 (since p < n) */
    if (n > 1)
        ensure_primes(n - 1);
    else
        prime_cnt = 0, free(primes), primes = NULL;

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* p-1 and p+1 practical */
        if (!ispractical[p - 1] || !ispractical[p + 1])
            continue;

        /* n - p must be prime or practical */
        idx_t rem = n - p;
        if (is_prime(rem) || (rem < practical_size && ispractical[rem]))
            ++count;
    }
    return count;
}
