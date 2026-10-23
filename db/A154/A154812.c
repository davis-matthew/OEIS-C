/*
 * A154812  Primes p such that the smallest decimal digit of p plus the
 *          smallest decimal digit of prime(p) is prime.
 * https://oeis.org/A154812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the n-th prime p such that smallest_decimal_digit(p) +
 *            smallest_decimal_digit(prime(p)) is a prime number. Sequence is
 *            memoised
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper to check if a small sum (0..18) is prime. */
static const bool is_small_sum_prime[19] = {
    false, /* 0 */
    false, /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    false, /* 6 */
    true,  /* 7 */
    false, /* 8 */
    false, /* 9 */
    false, /*10 */
    true,  /*11 */
    false, /*12 */
    true,  /*13 */
    false, /*14 */
    false, /*15 */
    false, /*16 */
    true,  /*17 */
    false  /*18 */
};

/* Find smallest decimal digit of x. */
static unsigned smallest_digit(seq_t x)
{
    unsigned best = 9;
    seq_t v = x;
    while (v > 0)
    {
        unsigned d = (unsigned)(v % 10);
        if (d < best)
            best = d;
        v /= 10;
    }
    return best;
}

/* Primes up to a static limit, generated once. */
#define SIEVE_LIMIT 5000000ULL

static uint64_t *prime_vals = NULL;   /* array of all primes up to SIEVE_LIMIT */
static size_t     prime_count = 0;     /* number of primes in prime_vals */
static size_t     prime_probe = 0;     /* next prime index to test for sequence */

/* Memoised sequence terms */
static seq_t    *seq_cache = NULL;
static size_t    seq_len   = 0;
static size_t    seq_cap   = 0;

/* One‑time sieve to initialise prime_vals and prime_count. */
static void init_primes(void)
{
    static bool done = false;
    if (done)
        return;
    done = true;

    /* Allocate temporary composite array */
    uint8_t *is_comp = (uint8_t*)calloc((size_t)SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (!is_comp)
        return;  /* In case of out‑of‑memory; function will then lack primes. */

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = 1;
    }

    /* Count primes and allocate prime_vals */
    size_t cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            ++cnt;

    prime_vals = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_vals)
    {
        free(is_comp);
        return;
    }
    prime_count = cnt;

    /* Fill prime_vals */
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
            prime_vals[idx++] = i;
    }

    free(is_comp);
}

/* Ensure seq_cache can hold at least needed elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Append val to the memoised sequence. */
static void append_val(seq_t val)
{
    ensure_seq_capacity(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for n=0, but handled safely */

    init_primes();   /* initialise primes once */

    /* Generate sequence terms until we have n terms. */
    while (seq_len < (size_t)n)
    {
        if (prime_probe >= prime_count)
        {
            /* In theory this should never happen for n <= 10000.
               If it does, we return 0. */
            return (seq_t)0;
        }

        seq_t p = (seq_t)prime_vals[prime_probe++];
        /* smallest digit of p */
        unsigned sd_p = smallest_digit(p);

        /* Ensure we have the p-th prime in prime_vals. */
        if ((size_t)p > prime_count)
        {
            /* For values beyond the current prime list this implementation
               cannot compute prime(p).  In practice this condition does not
               occur for the requested n. */
            continue;
        }

        seq_t q = prime_vals[(size_t)p - 1];   /* prime(p) */
        unsigned sd_q = smallest_digit(q);
        unsigned sum = sd_p + sd_q;

        if (sum < 19 && is_small_sum_prime[sum])
            append_val(p);
    }

    return seq_cache[(size_t)n - 1];
}
