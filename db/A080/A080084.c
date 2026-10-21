/*
 * A080084  Number of prime factors in the factorial of the n-th prime,
 *          counted with multiplicity.
 * https://oeis.org/A080084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 2262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A080084:  
   a(n) = number of prime factors (with multiplicity) in (pₙ)!,  
   where pₙ is the n‑th prime.  This equals Ω(pₙ!) =  
   Σ_{p ≤ pₙ} Σ_{k≥1} ⌊pₙ / pᵏ⌋.  */

/* Requires <math.h> for log().  */
#include <math.h>
#include <stdlib.h>

/* Global static storage for the sieve and prefix sums. */
static uint64_t *prime_list   = NULL;   /* p₁, p₂, … */
static size_t   prime_count   = 0;      /* number of primes stored */

static unsigned *spf          = NULL;   /* smallest prime factor for each i */
static seq_t   *omega_prefix  = NULL;   /* prefix sums of Ω(i) up to bound */

static size_t   sieve_limit   = 0;      /* current maximal index of spf and prefix */

/* ------------------------------------------------------------------ */
/* Rebuild the sieve up to 'limit'.  All global arrays are reallocated. */
/* ------------------------------------------------------------------ */
static void rebuild_sieve(size_t limit)
{
    /* free previously allocated memory */
    if (spf)          free(spf);
    if (omega_prefix) free(omega_prefix);
    if (prime_list)   free(prime_list);

    /* allocate new arrays */
    spf          = (unsigned*)malloc((limit + 1) * sizeof(unsigned));
    omega_prefix = (seq_t*)malloc((limit + 1) * sizeof(seq_t));

    /* dynamic array for primes */
    size_t prime_cap = 16;
    prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    prime_count = 0;

    /* linear sieve to fill spf[] and prime_list[] */
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                      /* i is prime */
            spf[i] = (unsigned)i;
            if (prime_count == prime_cap) {
                prime_cap <<= 1;                /* double capacity */
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = i;
        }
        for (size_t j = 0; j < prime_count &&
             prime_list[j] <= spf[i] &&
             i * prime_list[j] <= limit; ++j) {
            spf[i * prime_list[j]] = (unsigned)prime_list[j];
        }
    }

    /* temporary array for Ω(i) */
    seq_t *omega = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    omega[0] = 0;
    omega[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        omega[i] = 1 + omega[i / spf[i]];
    }

    /* prefix sums of Ω(i) */
    omega_prefix[0] = 0;
    for (size_t i = 1; i <= limit; ++i) {
        omega_prefix[i] = omega_prefix[i - 1] + omega[i];
    }

    free(omega);
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that we have at least 'n' primes and that the sieve covers
   up to the n‑th prime.  Uses an upper bound estimate for the n‑th
   prime when growth is needed.                                          */
/* ------------------------------------------------------------------ */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;   /* already sufficient */

    /* Upper bound for the n‑th prime (valid for n ≥ 6). */
    double dn = (double)n;
    size_t bound;
    if (n < 6)
        bound = 15;
    else
        bound = (size_t)(dn * (log(dn) + log(log(dn))) + 5 * dn);

    if (bound < sieve_limit)
        bound = sieve_limit;   /* already sufficient boundary */

    rebuild_sieve(bound);
}

/* ------------------------------------------------------------------ */
/* The required OEIS sequence function.                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, return 0 for safety */

    ensure_primes(n);

    /* pₙ is the (n‑1)‑th element of prime_list (0‑based). */
    uint64_t p_n = prime_list[n - 1];

    /* Ω(pₙ!) equals the prefix sum up to pₙ. */
    return omega_prefix[p_n];
}
