/*
 * A106551  a(n) = n-th semiprime + (concatenation of its two divisors,
 *          largest divisor first).
 * https://oeis.org/A106551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = nth_semiprime + (concatenation of its two divisors,
 *            largest divisor first)
 * Range:     u8 n <= 15, u16 n <= 1755, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for a simple SPF sieve and the list of semiprimes. */
static idx_t spf_size = 0;
static idx_t *spf = NULL;

static idx_t primes_cap = 0;
static idx_t primes_size = 0;
static idx_t *primes = NULL;

static size_t semis_cap = 0;
static size_t semis_size = 0;
static idx_t *semis = NULL;
static idx_t *fa = NULL;   /* larger divisor (largest of the two) */
static idx_t *fb = NULL;   /* smaller divisor */

/* Ensure that the smallest‑prime‑factor table covers all numbers up to 'limit' inclusive. */
static void ensure_sieve(idx_t limit)
{
    if (limit < spf_size)
        return;

    /* Allocate new space.  Cast from void* to idx_t* to satisfy C++ oddments. */
    idx_t new_size = limit + 1;
    idx_t *new_spf = (idx_t *)calloc((size_t)new_size, sizeof(idx_t));
    if (!new_spf)
        return; /* allocation failure – silently abort */

    /* Copy existing data if any. */
    if (spf) {
        for (idx_t i = 0; i < spf_size; ++i)
            new_spf[i] = spf[i];
        free(spf);
    }
    spf = new_spf;
    spf_size = new_size;

    /* Linear sieve for the new region. */
    for (idx_t i = 2; i < new_size; ++i) {
        if (spf[i] == 0) { /* i is prime */
            spf[i] = i;
            if (primes_size == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_size++] = i;
        }
        for (idx_t pj = 0; pj < primes_size; ++pj) {
            idx_t p = primes[pj];
            if (p > spf[i] || (idx_t)i * p >= new_size)
                break;
            spf[i * p] = p;
        }
    }
}

/* Test primality using the SPF table. */
static int is_prime_idx(idx_t x)
{
    if (x < 2)
        return 0;
    return spf[x] == x;
}

/* Compute 10^digits(x) as a seq_t value. */
static seq_t pow10_of_digits(idx_t x)
{
    seq_t p = 1;
    while (x > 0) {
        p *= 10;
        x /= 10;
    }
    return p;
}

/* Ensure that at least 'need' semiprimes are stored. */
static void ensure_semiprimes(size_t need)
{
    static idx_t current = 4;              /* 4 is the first semiprime */
    while (semis_size < need) {
        /* If we run out of SPF table, expand it. */
        if (current >= spf_size)
            ensure_sieve(current * 2 + 1000);

        idx_t p = spf[current];
        if (p != 0) {
            idx_t q = current / p;
            if (q >= 2 && is_prime_idx(q)) {
                /* Store this semiprime and its two factors. */
                if (semis_size == semis_cap) {
                    semis_cap = semis_cap ? semis_cap * 2 : 16;
                    semis = (idx_t *)realloc(semis, semis_cap * sizeof(idx_t));
                    fa   = (idx_t *)realloc(fa,   semis_cap * sizeof(idx_t));
                    fb   = (idx_t *)realloc(fb,   semis_cap * sizeof(idx_t));
                }
                semis[semis_size] = current;
                /* Largest factor first. */
                if (p >= q) {
                    fa[semis_size] = p;
                    fb[semis_size] = q;
                } else {
                    fa[semis_size] = q;
                    fb[semis_size] = p;
                }
                ++semis_size;
            }
        }
        ++current;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset is 1 */

    ensure_semiprimes((size_t)n);

    idx_t sp = semis[n - 1];
    idx_t larg = fa[n - 1];
    idx_t small = fb[n - 1];

    seq_t pow10 = pow10_of_digits(small);
    seq_t concat = (seq_t)larg * pow10 + (seq_t)small;

    return (seq_t)sp + concat;
}
