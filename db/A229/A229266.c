/*
 * A229266  Primes of the form sigma(k) + tau(k) + phi(k), where sigma(k) =
 *          A000203(k), tau(k) = A000005(k) and phi(k) = A000010(k).
 * https://oeis.org/A229266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 13, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   build_spf, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th prime that equals σ(k)+τ(k)+φ(k)
   (σ = sum of divisors, τ = number of divisors, φ = Euler totient).
   k is taken from 1 upwards; when the sum is prime it is recorded
   into the sequence.  Indexing is 1‑based. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions provided by the build environment      */
#include "../utils/build_spf.h"   /* build_spf(idx_t limit, idx_t **out) */
#include "../utils/is_prime_big.h"/* is_prime_big(seq_t n) */

/* Maximum expected size of the sequence for the required limits.
   20000 comfortably covers the first 10000 terms and leaves room
   for any future requests.  The value is a conservative upper bound
   – increase it if you encounter a memory overflow. */
#define SEQ_CAP 20000

/* Static storage for the first SEQ_CAP terms found. */
static seq_t seq_arr[SEQ_CAP] = {0};
static size_t seq_len = 0;       /* number of terms currently stored */

/* Smallest Prime Factor sieve information.                     */
static idx_t *spf = NULL;        /* array of smallest prime factors    */
static idx_t  spf_limit = 0;     /* largest index currently supported */

/* Number of k values already processed by the sieve.          */
static idx_t processed_k = 0;

/* -----------------------------------------------------------------*/
/* Helper: enlarge the SPF sieve so that it covers indices up to [1..limit].
   Returns true on success, false on memory allocation failure. */
static bool ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return true;                           /* already enough */

    idx_t *new_spf = NULL;
    build_spf(limit, &new_spf);                 /* may allocate memory      */
    if (!new_spf)
        return false;                           /* allocation failed        */

    free(spf);                                  /* discard old sieve */
    spf = new_spf;
    spf_limit = limit;
    return true;
}

/* -----------------------------------------------------------------*/
/* Factor a number k (k >= 1) using the current SPF array.
   Computes sigma(n), tau(n) and phi(n) and returns their sum.   */
static seq_t sum_sigma_tau_phi(idx_t k)
{
    idx_t remaining = k;
    seq_t sigma = 1;   /* product over σ(p^e)                */
    seq_t phi   = 1;   /* product over φ(p^e)                */
    seq_t tau   = 1;   /* product over (e+1)                */

    while (remaining > 1)
    {
        idx_t p = spf[remaining];
        if (p == 0)          /* safety: unknown factor           */
            p = remaining;

        seq_t p_pow = 1;          /* p^e                                */
        seq_t sigma_pe = 1;       /* Σ_{i=0..e} p^i                      */
        unsigned e = 0;

        /* factor out p^e                            */
        while (remaining % p == 0)
        {
            remaining /= p;
            ++e;
            p_pow *= (seq_t)p;
            sigma_pe += p_pow;
        }

        /* φ(p^e) = p^e − p^(e−1)                    */
        seq_t phi_pe = p_pow - (p_pow / (seq_t)p);

        sigma *= sigma_pe;
        phi   *= phi_pe;
        tau   *= (seq_t)(e + 1);
    }

    return sigma + tau + phi;
}

/* -----------------------------------------------------------------*/
/* Process k in the inclusive range [from .. to] (1‑based).  Every
   prime value found is appended to seq_arr, if there is space. */
static void process_range(idx_t from, idx_t to)
{
    for (idx_t k = from; k <= to; ++k)
    {
        seq_t value = sum_sigma_tau_phi(k);
        if (is_prime_big(value))
        {
            if (seq_len < SEQ_CAP)
                seq_arr[seq_len++] = value;      /* store term       */
            /* if seq_arr is full we silently discard further terms – 
               the build environment will not request more than SEQ_CAP */
        }
    }
}

/* -----------------------------------------------------------------*/
/* Public function required by all builds.                           */
seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence offset is 1   */
        return 0;

    /* Return cached value if already computed. */
    if (n <= seq_len)
        return seq_arr[n - 1];

    /* Determine an initial sieve size.  We start with 1,000,000;
       should that prove inadequate we enlarge by doubling. */
    const idx_t INITIAL_SIEVE = 1000000;
    if (!ensure_spf(INITIAL_SIEVE))
        return 0;                    /* allocation failure    */

    /* Process k values in chunks until we have at least n terms. */
    const idx_t CHUNK_SIZE = 100000;
    while (seq_len < n)
    {
        /* If we have already processed the entire current sieve, enlarge it. */
        if (processed_k >= spf_limit)
        {
            idx_t new_limit = spf_limit * 2;          /* double size   */
            if (!ensure_spf(new_limit))
                return 0;                            /* allocation fail */
        }

        /* Determine next block of k values to examine. */
        idx_t from = processed_k + 1;
        idx_t to   = processed_k + CHUNK_SIZE;
        if (to > spf_limit)
            to = spf_limit;

        process_range(from, to);
        processed_k = to;

        /* If the sieve has run out of numbers before we find enough terms,
           enlarge it further. */
        if (processed_k == spf_limit && seq_len < n)
            continue;          /* loop will re‑check and enlarge */
    }

    return seq_arr[n - 1];
}
