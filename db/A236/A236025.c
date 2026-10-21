/*
 * A236025  Composite numbers n sorted by increasing values of delta(n) =
 *          (n+1)^(1/2) - sigma(n)^(1/tau(n)), where sigma(n) = A000203(n) =
 *          the sum of divisors of n and tau(n) = A000005(n) = the number of
 *          divisors of n.
 * https://oeis.org/A236025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th composite number sorted by increasing delta(n)
 *            = sqrt(n+1) - sigma(n)^(1/tau(n)), where sigma is the sum of
 *            divisors and tau is the divisor count
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all composite numbers up to a
   fixed upper bound, calculates delta with double precision,
   sorts the numbers, and stores the resulting order in a static
   array.  Queries are answered by indexing this array.  The
   precomputation is performed only once, so subsequent calls are O(1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

/* type aliases are supplied by the build system (seq_t, idx_t). */

/* ---------- helper data structures ------------------------------------ */
typedef struct {
    uint32_t n;   /* composite number */
    double   delta;   /* its delta value */
} entry_t;

/* ---------- static tables and state ---------------------------------- */
static bool          initialized = false;
static size_t        capacity    = 0;   /* current allocated capacity */
static size_t        count       = 0;   /* number of entries stored */
static entry_t      *entries     = NULL; /* array of {n, delta} */
static seq_t       *sorted_vals = NULL;   /* sorted composite numbers */

/* ---------- small prime sieve ---------------------------------------- */
static uint32_t *prime_factor = NULL; /* smallest prime factor array */
static size_t    prime_size   = 0;    /* size of the array */

/* Build smallest prime factor array up to limit. */
static void build_spf(uint32_t limit)
{
    if (prime_size > limit) return;   /* already sufficient */

    free(prime_factor);
    prime_size = limit + 1;
    prime_factor = (uint32_t *)malloc(prime_size * sizeof(uint32_t));
    if (!prime_factor) exit(EXIT_FAILURE);

    for (uint32_t i = 0; i <= limit; ++i) prime_factor[i] = i;

    for (uint32_t p = 2; (uint64_t)p * p <= limit; ++p) {
        if (prime_factor[p] != p) continue; /* composite */
        for (uint64_t j = (uint64_t)p * p; j <= limit; j += p) {
            if (prime_factor[j] == j) prime_factor[j] = p;
        }
    }
}

/* Compute sigma(n) and tau(n) using the precomputed spf. */
static void sigma_tau(uint32_t n, uint64_t *sigma, uint64_t *tau)
{
    uint64_t s = 1;
    uint64_t t = 1;
    uint32_t m = n;
    while (m > 1) {
        uint32_t p = prime_factor[m];
        uint64_t cnt = 0;
        uint64_t p_pow = 1;
        while (m % p == 0) {
            m /= p;
            ++cnt;
            p_pow *= p;
        }
        /* term = (p^(cnt+1)-1)/(p-1) */
        uint64_t term = (p_pow * p - 1) / (p - 1);
        s *= term;
        t *= (cnt + 1);
    }
    *sigma = s;
    *tau   = t;
}

/* Ensure the entries array has enough capacity. */
static void ensure_capacity(size_t needed)
{
    if (capacity >= needed) return;
    size_t new_cap = capacity ? capacity * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    entry_t *tmp = (entry_t *)realloc(entries, new_cap * sizeof(entry_t));
    if (!tmp) exit(EXIT_FAILURE);
    entries = tmp;
    capacity = new_cap;
}

/* Comparator for qsort: primary by delta, then by n. */
static int cmp_entries(const void *a, const void *b)
{
    const entry_t *ea = (const entry_t *)a;
    const entry_t *eb = (const entry_t *)b;
    if (ea->delta < eb->delta) return -1;
    if (ea->delta > eb->delta) return 1;
    if (ea->n < eb->n) return -1;
    if (ea->n > eb->n) return 1;
    return 0;
}

/* Precompute all composites up to LIMIT, sort them, and store the
   resulting order in sorted_vals. */
static void precompute(void)
{
    const uint32_t LIMIT = 400000;  /* sufficient for n up to ~10^4 */
    build_spf(LIMIT);

    /* Reserve enough capacity for all composites up to LIMIT. */
    size_t est_composites = LIMIT - 1; /* upper bound */
    ensure_capacity(est_composites);

    for (uint32_t n = 4; n <= LIMIT; ++n) {
        if (prime_factor[n] == n) continue; /* skip primes */
        uint64_t sigma, tau;
        sigma_tau(n, &sigma, &tau);
        double delta = sqrt((double)n + 1.0) -
                       pow((double)sigma, 1.0 / (double)tau);
        ensure_capacity(count + 1);
        entries[count].n = n;
        entries[count].delta = delta;
        ++count;
    }

    /* Sort the entries by delta. */
    qsort(entries, count, sizeof(entry_t), cmp_entries);

    /* Extract the sorted numbers into sorted_vals. */
    sorted_vals = (seq_t *)malloc(count * sizeof(seq_t));
    if (!sorted_vals) exit(EXIT_FAILURE);
    for (size_t i = 0; i < count; ++i) {
        sorted_vals[i] = (seq_t)entries[i].n;
    }

    initialized = true;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but safe */
    if (!initialized) precompute();
    /* The sequence is 1‑based. */
    return sorted_vals[n - 1];
}
