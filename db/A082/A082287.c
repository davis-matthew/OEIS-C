/*
 * A082287  a(1) = 1; for n > 1, n appears omega(n) times, where
 *          omega(n)=A001221(n) is the number of distinct prime factors of n,
 *          a(1)=1.
 * https://oeis.org/A082287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082287: a(n) is the smallest k with Σ_{i=2}^{k} ω(i) ≥ n‑1
   (ω(i) = number of distinct prime factors of i).  a(1)=1.  Thus
   each integer m appears ω(m) times in the sequence.  The function
   performs an on‑demand sieve of smallest prime factors to
   build ω and its cumulative sum, then binary searches the
   cumulative array. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *spf = NULL;          /* smallest prime factor */
static unsigned char *omega_arr = NULL; /* distinct‑prime count per index   */
static seq_t *cum = NULL;             /* cumulative sums of omega          */
static idx_t capacity   = 0;           /* size of allocated tables          */
static idx_t max_computed = 0;         /* highest index that has been filled */
static bool initialized = false;       /* one‑time init flag                */

/* Compute smallest prime factor array for numbers 2..limit */
static void compute_spf(idx_t limit)
{
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)           /* mark first (smallest) prime */
                        spf[j] = (uint32_t)i;
            }
        }
    }
    /* Any primes > sqrt(limit) that were not yet set */
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0)
            spf[i] = (uint32_t)i;
}

/* Compute ω (distinct prime factor count) up to limit */
static void compute_omega(idx_t limit)
{
    omega_arr[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t p = (idx_t)spf[i];
        idx_t m = i / p;
        if (m % p == 0)
            omega_arr[i] = omega_arr[m];
        else
            omega_arr[i] = omega_arr[m] + 1;
    }
}

/* Compute cumulative sums Σ_{i=2}^{k} ω(i) up to limit */
static void compute_cum(idx_t limit)
{
    cum[0] = 0;
    cum[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        cum[i] = cum[i - 1] + (seq_t)omega_arr[i];
}

/* Allocate or grow tables to at least 'limit' elements */
static void ensure_capacity(idx_t limit)
{
    if (limit <= max_computed)
        return;                                 /* already ready */

    idx_t new_capacity = limit;
    if (capacity > 0 && new_capacity < capacity * 2)
        new_capacity = capacity * 2;            /* grow roughly twice */

    /* Reallocate buffers */
    uint32_t *new_spf   = (uint32_t *)realloc(spf,   sizeof(uint32_t)   * (new_capacity + 1));
    unsigned char *new_omega = (unsigned char *)realloc(omega_arr, sizeof(unsigned char) * (new_capacity + 1));
    seq_t *new_cum     = (seq_t *)realloc(cum,   sizeof(seq_t)     * (new_capacity + 1));

    if (!new_spf || !new_omega || !new_cum)
        abort();                                 /* out of memory */

    spf   = new_spf;
    omega_arr = new_omega;
    cum   = new_cum;
    capacity = new_capacity;

    /* Clear newly allocated memory so that SPF scan works correctly */
    memset(spf, 0, sizeof(uint32_t) * (capacity + 1));
    memset(omega_arr, 0, sizeof(unsigned char) * (capacity + 1));
    memset(cum, 0, sizeof(seq_t) * (capacity + 1));

    /* Recompute all tables for the new range */
    compute_spf(capacity);
    compute_omega(capacity);
    compute_cum(capacity);
    max_computed = capacity;
}

/* One‑time initialization with a modest starting size */
static void init_sequence(void)
{
    if (initialized)
        return;
    const idx_t INITIAL = 20000;
    ensure_capacity(INITIAL);
    initialized = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                       /* explicit definition */

    init_sequence();

    /* Find sufficient range to contain the cumulative sum equal to n‑1 */
    seq_t target = (seq_t)(n - 1);
    while (cum[max_computed] < target)          /* not yet far enough */
        ensure_capacity(max_computed * 2 > max_computed ? max_computed * 2 : max_computed + 1);

    /* Binary search for minimal k with cum[k] >= target */
    idx_t lo = 2;
    idx_t hi = max_computed;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
