/*
 * A029896  Convolution of Thue-Morse sequence A001285 with A008578 = {1,
 *          primes}.
 * https://oeis.org/A029896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^n (1 + T(k)) ⋅ p_{n‑k} (OEIS A029896) T(k) –
 *            Thue–Morse value (0 if popcount(k) even, 1 if odd) p_i – A008578
 *            sequence (1, 2, 3, 5, 7, 11, …)
 * Range:     u8 n <= 11, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static seq_t *a_cache = NULL;   /* convolution result */
static seq_t *p_cache = NULL;   /* p_i = {1, primes} */
static seq_t *w_cache = NULL;   /* weights 1 or 2 */
static idx_t computed_max = 0;
static int initialized = 0;

/* Return the first need primes (excluding the leading 1).
   Primes are stored in the vector 'primes', which is passed in as a
   preallocated seq_t array of size need.  */
static void build_primes(size_t need, seq_t *primes)
{
    if (need == 0) return;

    /* upper bound for the nth prime (n >= 6) */
    size_t bound;
    if (need < 6)
        bound = 15;
    else
    {
        double n = (double)need;
        bound = (size_t)(n * (log(n) + log(log(n))) + 3.0);
    }

    /* simple Eratosthenes sieve */
    char *is_prime = (char*)malloc(bound + 1);
    for (size_t i = 0; i <= bound; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= bound; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= bound; j += i)
                is_prime[j] = 0;

    size_t cnt = 0;
    for (size_t i = 2; i <= bound && cnt < need; ++i)
    {
        if (is_prime[i])
        {
            primes[cnt++] = (seq_t)i;
        }
    }
    free(is_prime);
}

seq_t a(idx_t n)
{
    /* Recompute if a new maximum index is requested */
    if (!initialized || n > computed_max)
    {
        idx_t new_max = n;
        size_t sz = (size_t)new_max + 1;

        /* Allocate new arrays */
        seq_t *new_a = (seq_t*)calloc(sz, sizeof(seq_t));
        seq_t *new_p = (seq_t*)calloc(sz, sizeof(seq_t));
        seq_t *new_w = (seq_t*)calloc(sz, sizeof(seq_t));

        /* --- Build weight array w_i = 1 + popcount(i) mod 2 (values 1 or 2) --- */
        for (size_t i = 0; i < sz; ++i)
        {
            unsigned int pc = __builtin_popcountll((unsigned long long)i);
            new_w[i] = 1u + (pc & 1u);
        }

        /* --- Build prime + 1 array p_i --- */
        new_p[0] = 1;                /* leading 1 */
        size_t primes_needed = (size_t)new_max;   /* number of primes after the leading 1 */
        if (primes_needed > 0)
        {
            /* primes_temp will hold the needed primes */
            seq_t *primes_temp = (seq_t*)malloc(primes_needed * sizeof(seq_t));
            build_primes(primes_needed, primes_temp);
            for (size_t i = 0; i < primes_needed; ++i)
                new_p[i + 1] = primes_temp[i];
            free(primes_temp);
        }

        /* --- Convolution: a(i + j) += w[i] * p[j] --- */
        for (size_t i = 0; i < sz; ++i)
        {
            seq_t wi = new_w[i];
            size_t jmax = sz - 1 - i;
            for (size_t j = 0; j <= jmax; ++j)
            {
                new_a[i + j] += wi * new_p[j];
            }
        }

        /* Free old arrays if any */
        if (initialized)
        {
            free(a_cache);
            free(p_cache);
            free(w_cache);
        }

        /* Store new arrays */
        a_cache = new_a;
        p_cache = new_p;
        w_cache = new_w;

        computed_max = new_max;
        initialized = 1;
    }

    return a_cache[(size_t)n];
}
