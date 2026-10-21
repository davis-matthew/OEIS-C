/*
 * A087874  a[n] =PrimePi[PrimePi[n-1]] + a[n - a[n-1]].
 * https://oeis.org/A087874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi[PrimePi[n-1]] + a[n - a[n-1]] (a(1) = 1)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve and PrimePi (π(x)) helper                               */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list      = NULL;   /* array of primes          */
static size_t   prime_cap        = 0;      /* capacity of prime_list   */
static size_t   prime_count      = 0;      /* number of primes stored  */
static idx_t    prime_limit      = 1;      /* largest integer checked  */

static void grow_primes(size_t need)
{
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        if (new_cap < need) new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

static void extend_sieve(idx_t up_to)
{
    if (prime_limit >= up_to) return;
    for (idx_t n = prime_limit + 1; n <= up_to; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_primes(prime_count + 1);
            prime_list[prime_count++] = n;
        }
    }
    prime_limit = up_to;
}

static size_t prime_pi(idx_t x)
{
    if (x < 2) return 0;
    size_t lo = 0, hi = prime_count;      /* [lo,hi) interval of indices */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= x) lo = mid + 1;
        else hi = mid;
    }
    return lo; /* number of primes <= x */
}

/* ------------------------------------------------------------------ */
/*  Dynamic array for sequence values                                 */
/* ------------------------------------------------------------------ */
static seq_t *seq_vals      = NULL;   /* a[0] unused, a[1] = 1 */
static size_t seq_cap       = 0;      /* capacity in seq_t elements */
static size_t seq_count     = 0;      /* largest index stored (>=1) */

static void grow_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_vals  = tmp;
    seq_cap   = new_cap;
}

/* Ensure that `seq_vals` contains a[1]..a[n] (n>=1) */
static void ensure_seq(idx_t n)
{
    if (n <= seq_count) return;

    /* Extend prime sieve to `n-1` because recurrence uses a[n-1] */
    extend_sieve((idx_t)(n - 1));

    /* Grow sequence array */
    grow_seq((size_t)(n + 1));          /* need index n */

    /* If first time, initialise a[1] */
    if (seq_count == 0) {
        seq_vals[0] = 0;                /* a[0] unused */
        seq_vals[1] = 1;                /* a(1)=1 */
        seq_count  = 1;
    }

    /* Compute new terms */
    for (idx_t i = seq_count + 1; i <= n; ++i) {
        idx_t p1 = prime_pi(i - 1);          /* π(i‑1) */
        idx_t p2 = prime_pi(p1);            /* π(π(i‑1)) */
        idx_t idx = i - (idx_t)seq_vals[i - 1]; /* i - a[i‑1] */
        /* Safety: idx must be >=1 and <= i-1 */
        if (idx < 1 || idx > i - 1) idx = 1;
        seq_vals[i] = (seq_t)p2 + seq_vals[(size_t)idx];
    }
    seq_count = (size_t)n;
}

/* ------------------------------------------------------------------ */
/*  Public function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_seq(n);
    return seq_vals[(size_t)n];
}
