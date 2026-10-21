/*
 * A082896  a(n) = A082893(n)/n.
 * https://oeis.org/A082896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((floor(n/2) + p(n))/n)
 * Range:     u8 n <= 50, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(n) denotes the n-th prime.   */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* seq_t and idx_t are provided by the build environment. */
static uint32_t *prime_list = NULL;          /* stores the primes (1‑based index)   */
static size_t   prime_count = 0;             /* number of primes stored           */
static char *primality_bitmap = NULL;       /* sieve storage                     */
static size_t   bitmap_size = 0;             /* size of the bitmap                */

/* ------------------------------------------------------------------ */
/*  Simple sieve of Eratosthenes that fills prime_list and prime_count  */
/* ------------------------------------------------------------------ */
static void build_primes_up_to(size_t limit)
{
    if (primality_bitmap) { free(primality_bitmap); }
    primality_bitmap = (char*)calloc(limit + 1, 1);   /* 0 indicates composite  */
    if (!primality_bitmap) return; /* out of memory */

    if (limit >= 0) primality_bitmap[0] = primality_bitmap[1] = 1;
    for (size_t i = 2; i * i <= limit; ++i)
        if (!primality_bitmap[i])
            for (size_t j = i * i; j <= limit; j += i)
                primality_bitmap[j] = 1;

    if (prime_list) { free(prime_list); }
    /* Estimate number of primes: n / log n  + 10 for safe margin */
    size_t estimated = (size_t)(limit / 10) + 100;
    prime_list = (uint32_t*)malloc(estimated * sizeof(uint32_t));
    if (!prime_list) return; /* out of memory */

    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!primality_bitmap[i]) {
            if (prime_count == estimated) {
                /* double the array if needed */
                estimated *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, estimated * sizeof(uint32_t));
                if (!tmp) return;  /* out of memory */
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
    bitmap_size = limit + 1;
}

/* ------------------------------------------------------------------ */
/*  Helper to guarantee we have at least n primes available           */
/* ------------------------------------------------------------------ */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    /* Rough upper bound for the n-th prime (valid for n >= 6)          */
    size_t lim;
    if (n < 6) {
        lim = 15;
    } else {
        double dn = (double)n;
        double bound = dn * (log(dn) + log(log(dn))) + 10.0;
        lim = (size_t)bound;
    }
    if (lim < n * 2) lim = n * 2;     /* just in case the estimate is low   */
    build_primes_up_to(lim);
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but guard against 0 */

    ensure_primes(n);                /* make sure the n-th prime exists   */

    seq_t half = ((seq_t)n) / 2;     /* floor(n/2) */
    seq_t pn   = (seq_t)prime_list[n - 1];   /* p(n) */
    seq_t sum  = half + pn;
    seq_t res  = sum / ((seq_t)n);
    return res;
}
