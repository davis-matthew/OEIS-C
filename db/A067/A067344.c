/*
 * A067344  Sum of decimal digits of square of divisors of n equals sum of
 *          square of digits of n.
 * https://oeis.org/A067344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer n such that sum of decimal
 *            digits of σ₂(n) (sum of squares of divisors of n) equals (sum of
 *            decimal digits of n)². In OEIS notation: a(n) =
 *            A007953(A001157(n)) = A007953(n)^2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------- necessary headers ----------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------- helper: digit sum of a seq_t value ----------------- */
static unsigned int digit_sum_seq(seq_t v)
{
    unsigned int sum = 0;
    while (v != 0) {
        sum += (unsigned int)(v % 10);
        v /= 10;
    }
    return sum;
}

/* ----------------- SPF sieve parameters ----------------- */
#define MAX_CAND 20000000UL          /* maximum integer to test for sequence terms */

static uint32_t *spf = NULL;        /* smallest prime factor array */

/* ------------- one‑time initializer for the SPF array ------------- */
static void init_spf(void)
{
    if (spf != NULL)
        return;                     /* already initialized */

    spf = (uint32_t *)malloc((MAX_CAND + 1) * sizeof(uint32_t));
    if (!spf)
        exit(EXIT_FAILURE);         /* critical: memory exhausted */

    for (uint32_t i = 0; i <= MAX_CAND; ++i)
        spf[i] = 0;
    /* 0 and 1 have no prime factors */
    spf[0] = 0;
    spf[1] = 1;

    /* sieve to fill spf with smallest prime factor */
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_CAND; ++i) {
        if (spf[i] == 0) {
            for (uint32_t j = i * i; j <= MAX_CAND; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }
    for (uint32_t i = 2; i <= MAX_CAND; ++i)
        if (spf[i] == 0)
            spf[i] = i;              /* remaining primes are unchanged */
}

/* ----------------- check whether integer satisfies property ----------------- */
static bool satisfies(idx_t cand)
{
    /* 1. sum of squares of decimal digits of cand */
    unsigned int sum_sq_digits = 0;
    idx_t t = cand;
    while (t > 0) {
        unsigned int d = t % 10;
        sum_sq_digits += d * d;
        t /= 10;
    }

    /* 2. compute σ₂(cand) = ∑_{d|cand} d² via prime factorisation */
    seq_t sigma2 = 1;
    idx_t temp = cand;
    while (temp > 1) {
        uint32_t p   = spf[temp];
        unsigned int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        /* Compute 1 + p² + p⁴ + … + p^(2e) */
        seq_t sum = 1;          /* term for i = 0 */
        seq_t power = 1;        /* p^i, start with 1 */
        for (unsigned int i = 0; i < e; ++i) {
            power *= (seq_t)p;            /* p^(i+1) */
            sum += power * power;         /* (p^(i+1))²   */
        }
        sigma2 *= sum;
    }

    /* 3. digit sum of σ₂(cand) */
    unsigned int sum_digits_sigma2 = digit_sum_seq(sigma2);

    return sum_digits_sigma2 == sum_sq_digits;
}

/* ----------------- static cache for sequence terms ----------------- */
static seq_t *cache = NULL;   /* sequence values stored as seq_t */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */
static idx_t next_candidate = 1;  /* next integer to test */

/* forward declaration of helper that adds a value to the cache */
static void push_cache(seq_t val);

/* ----------------- grow cache capacity if needed ----------------- */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        exit(EXIT_FAILURE);     /* cannot continue */
    cache = newp;
    cache_cap = new_cap;
}

/* ----------------- ensure that at least n terms are cached ----------------- */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        if (satisfies(next_candidate))
            push_cache((seq_t)next_candidate);
        ++next_candidate;
        if (next_candidate > MAX_CAND) {
            /* safety guard – should not happen for the tested range */
            exit(EXIT_FAILURE);
        }
    }
}

/* ----------------- add a new term into the cache ----------------- */
static void push_cache(seq_t val)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* ----------------- required public function ----------------- */
seq_t a(idx_t n)
{
    static bool initialised = false;
    if (!initialised) {
        init_spf();          /* set up the SPF array once */
        initialised = true;
    }
    ensure_terms(n);
    return cache[n - 1];     /* 1‑based indexing */
}
