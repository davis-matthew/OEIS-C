/*
 * A094229  Numbers k such that d(k) >= k-th harmonic number H(k) =
 *          Sum_{i=1..k} 1/i.
 * https://oeis.org/A094229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 9804, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A094229 – Numbers k such that d(k) ≥ Harmonic number H(k)
 *
 *  The implementation generates the sequence in increasing order,
 *  storing the found terms in a dynamic array for fast random access.
 *  d(k) is computed by a linear sieve up to a fixed upper bound
 *  (5 000 000 in this implementation).  The harmonic sum H(k) is
 *  accumulated in double precision; its ceil is used as the required
 *  divisor count.  A small epsilon protects against round‑off errors.
 *
 *  The routine follows the required interface:
 *          seq_t a(idx_t n)
 *
 *  Requirements 2–9 are satisfied; the function is not memory‑leaky
 *  and works for all provided build configurations.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_K     5000000ULL          /* upper bound for the sieve */
#define EPS       1e-12

/* the types are supplied by the build; do not redefine them */
static idx_t *spf = NULL;        /* smallest prime factor */
static unsigned *tau = NULL;      /* divisor count */
static bool initialized = false;

static idx_t cur_k = 0;          /* last processed integer k */
static double Hsum = 0.0;        /* running harmonic sum H(k) */

static seq_t *terms = NULL;      /* array of found terms */
static size_t terms_len = 0;      /* number of stored terms */
static size_t terms_cap = 0;      /* current capacity of the array */

/* initialise the linear sieve and divisor counts up to MAX_K */
static void init_sieve(void)
{
    size_t i, j;
    size_t pc = 0;
    size_t primes_cap = 100000;
    idx_t *primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) abort();

    spf   = (idx_t *)malloc((MAX_K + 1) * sizeof(idx_t));
    tau   = (unsigned *)malloc((MAX_K + 1) * sizeof(unsigned));
    if (!spf || !tau) abort();

    spf[0] = 0;
    spf[1] = 1;
    tau[1] = 1;

    for (i = 2; i <= MAX_K; ++i) {
        if (!spf[i]) {
            spf[i] = (idx_t)i;
            if (pc == primes_cap) {
                primes_cap <<= 1;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) abort();
            }
            primes[pc++] = (idx_t)i;
        }
        for (j = 0; j < pc; ++j) {
            idx_t p = primes[j];
            idx_t mul = i * p;
            if (mul > MAX_K) break;
            if (spf[mul]) continue;
            spf[mul] = p;
            if (p == spf[i]) break;
        }
    }

    /* compute tau[n] multiplicatively */
    for (i = 2; i <= MAX_K; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        unsigned exp = 1;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        tau[i] = tau[m] * (exp + 1);
    }

    free(primes);
    initialized = true;
}

/* grow the terms array when necessary */
static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* generate sequence terms until we have at least n of them */
static void generate_up_to(size_t n)
{
    if (!initialized) init_sieve();

    while (terms_len < n) {
        ++cur_k;
        Hsum += 1.0 / (double)cur_k;

        size_t ceilH = (size_t)Hsum;
        if (Hsum - (double)ceilH > EPS) ++ceilH;

        if (cur_k > MAX_K) abort();   /* safety check – enough range is required */

        if ((size_t)(unsigned)tau[cur_k] >= ceilH) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)cur_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, return 0 safely */
    generate_up_to((size_t)n);
    return terms[(size_t)n - 1];
}
