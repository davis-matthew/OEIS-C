/*
 * A095702  Smallest "n-augmented" amicable number: the smallest positive
 *          integer k such that m = sigma(k) - k + n > k and k = sigma(m) - m
 *          + n, where sigma(k) is the sum of the divisors of k.
 * https://oeis.org/A095702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that * m = sigma(k) – k + n > k and
 *            sigma(m) – m + n = k * * The program searches for k by
 *            increasing trial values. Sigma is * computed via prime
 *            factorisation using a static sieve of Eratosthenes * up to a
 *            fixed limit (2 000 000), which is sufficient for the *
 *            required...
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*  Prime generation ----------------------------------------------------- */
static int  *prime_list  = NULL;   /* array of primes */
static size_t prime_count = 0;     /* number of primes */

static void init_primes(void)
{
    static int ready = 0;
    if (ready) return;

    /* Sieve up to 2 000 000 – enough for all trial divisions needed. */
    const int LIM = 2000000;
    unsigned char *is_composite =
        (unsigned char *)calloc(LIM + 1, sizeof(unsigned char));
    if (!is_composite) return;   /* allocation failure handled silently */

    prime_list  = (int *)malloc((LIM / 2 + 10) * sizeof(int));
    if (!prime_list) { free(is_composite); return; }

    size_t cnt = 0;
    for (int i = 2; i <= LIM; ++i) {
        if (!is_composite[i]) {
            prime_list[cnt++] = i;
            if ((int64_t)i * i <= LIM) {
                for (int j = i * i; j <= LIM; j += i) {
                    is_composite[j] = 1;
                }
            }
        }
    }
    prime_count = cnt;
    free(is_composite);
    ready = 1;
}

/* --------------------------------------------------------------------- */
/*  Sigma function for uint64_t argument -------------------------------- */
static seq_t sigma_u64(uint64_t n)
{
    if (n <= 1) return (seq_t)1;  /* sigma(1)=1 */

    uint64_t tmp = n;
    seq_t    result = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i) {
        int p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        if (tmp % p == 0) {
            seq_t term  = (seq_t)1;
            seq_t power = (seq_t)1;
            while (tmp % p == 0) {
                tmp   /= p;
                power *= (seq_t)p;
                term  += power;
            }
            result *= term;
        }
    }
    if (tmp > 1) {
        /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)tmp);
    }
    return result;
}

/* --------------------------------------------------------------------- */
/*  Cache for already computed values ----------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0;

/* --------------------------------------------------------------------- */
/*  Main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    /* expand cache if necessary */
    if ((size_t)n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_cache =
            (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        /* initialise new slots to zero */
        memset(&new_cache[cache_sz], 0,
               (new_sz - cache_sz) * sizeof(seq_t));
        cache = new_cache;
        cache_sz = new_sz;
    }

    /* return cached result if already computed */
    if (cache[n] != (seq_t)0) return cache[n];

    init_primes();

    /* search for the smallest k */
    for (uint64_t k = 1; ; ++k) {
        seq_t sigma_k = sigma_u64(k);
        seq_t m = sigma_k - (seq_t)k + (seq_t)n;
        if (m <= (seq_t)k) continue;   /* m must be > k */

        /* m must fit in uint64_t for the next sigma calculation */
        uint64_t m64 = (uint64_t)m;
        seq_t sigma_m = sigma_u64(m64);
        seq_t k2 = sigma_m - (seq_t)m64 + (seq_t)n;
        if (k2 == (seq_t)k) {
            cache[n] = (seq_t)k;
            return cache[n];
        }
        /* loop continues until a match is found – the definition guarantees
           existence for all n ≥ 0 */
    }
}
