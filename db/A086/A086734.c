/*
 * A086734  Convolution of the prime numbers with phi(n).
 * https://oeis.org/A086734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} prime(k) * φ(n+1-k)
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  prime(k)   – the k‑th prime number,            */
/*  φ(m)       – Euler's totient function.         */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- static storage ------------------------------------------- */
static idx_t  max_n_phi  = 0;  /* maximum n for which φ[0..n] are known   */
static seq_t *phi_arr    = NULL;  /* array of φ(0) … φ(max_n_phi)          */

static idx_t  prime_count = 0;  /* number of primes currently stored      */
static idx_t *prime_arr   = NULL;  /* array of primes                       */
static idx_t  prime_limit = 0;  /* sieve limit used for the last call     */

/* ---------- Primes --------------------------------------------------- */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed)
        return;                                   /* already have enough */

    /* Estimate a sieve limit that contains at least 'needed' primes.
       For n ≥ 6: p_n < n (ln n + ln ln n).  For very small n use a small
       fixed upper bound. */
    idx_t limit;
    if (needed < 6)
        limit = 15;             /* gives primes up to 13 (6 primes) */
    else {
        double nd = (double)needed;
        double bound = nd * (log(nd) + log(log(nd))) + 3.0;
        limit = (idx_t)bound;
    }

    /* Allocate boolean array for the sieve. */
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp)
        return;   /* allocation failure, unlikely for the problem size */

    /* Allocate space for the primes. */
    size_t new_cap = needed;
    prime_arr = (idx_t *)realloc(prime_arr, new_cap * sizeof(idx_t));
    if (!prime_arr) {
        free(is_comp);
        return;
    }
    prime_count = 0;

    /* Simple sieve of Eratosthenes.  We record primes until the
       required count is reached. */
    for (idx_t i = 2; i <= limit && prime_count < needed; ++i) {
        if (!is_comp[i]) {
            prime_arr[prime_count++] = i;

            /* Mark multiples of i as composite. */
            if (i <= limit / 2) {
                for (idx_t j = i + i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);
    prime_limit = limit;
}

/* ---------- Euler's Totient ---------------------------------------- */
static void ensure_phi(idx_t upto)
{
    if (upto <= max_n_phi)          /* already computed up to this value */
        return;

    /* Resize the φ array. */
    seq_t *new_phi = (seq_t *)realloc(phi_arr, (upto + 1) * sizeof(seq_t));
    if (!new_phi) return;           /* allocation failure, unlikely */

    /* Initialize φ(i)=i and φ(0)=0, φ(1)=1. */
    for (idx_t i = 0; i <= upto; ++i) new_phi[i] = (seq_t)i;
    new_phi[0] = (seq_t)0;
    new_phi[1] = (seq_t)1;

    /* Boolean array for marking composites during the sieve. */
    bool *is_comp = (bool *)calloc(upto + 1, sizeof(bool));
    if (!is_comp) {
        phi_arr = new_phi;
        max_n_phi = upto;
        return;
    }

    /* Compute φ using the standard sieve method. */
    for (idx_t i = 2; i <= upto; ++i) {
        if (!is_comp[i]) {               /* i is prime */
            for (idx_t j = i; j <= upto; j += i)
                new_phi[j] -= new_phi[j] / i;

            /* Mark multiples of i as composite. */
            if (i <= upto / 2) {
                for (idx_t j = i + i; j <= upto; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);

    phi_arr   = new_phi;
    max_n_phi = upto;
}

/* ---------- Sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n ≥ 1 */

    /* Prepare data structures. */
    ensure_primes((size_t)n);            /* first n primes are needed */
    ensure_phi(n);                       /* φ up to n is needed */

    /* Compute the convolution. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t prime_k = (seq_t)prime_arr[k - 1];          /* prime(k)   */
        seq_t phi_val = phi_arr[n + 1 - k];              /* φ(n+1-k)   */
        sum += prime_k * phi_val;
    }

    return sum;
}
