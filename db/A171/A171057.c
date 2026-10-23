/*
 * A171057  In the sequence of prime numbers, replace all the '9' digits with
 *          '8' and vice versa.
 * https://oeis.org/A171057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime with all decimal 8s and 9s swapped (e.g.,
 *            29 becomes 28, 89 becomes 98)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* Helper: swap 8↔9 in the decimal representation of x               */
/* ------------------------------------------------------------------ */
static inline seq_t transform_digits(seq_t x)
{
    seq_t result = (seq_t)0;
    seq_t mul = (seq_t)1;
    const seq_t ten = (seq_t)10;

    while (x > (seq_t)0)
    {
        uint8_t d = (uint8_t)(x % ten);
        if (d == 8) d = 9;
        else if (d == 9) d = 8;
        result += (seq_t)d * mul;
        mul *= ten;
        x /= ten;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Prime generation utilities                                         */
/* ------------------------------------------------------------------ */

/* Estimate an upper bound for the nth prime (Rosser, 1938). */
static uint64_t upper_bound_nth_prime(idx_t n)
{
    if (n < 6)
        return 15;                       /* small hard‑coded bound */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (uint64_t)bound + 10ULL;      /* safety margin */
}

/* Ensure that we have at least n primes stored in the static array. */
static idx_t *primes = NULL;          /* array of primes as uint64_t (idx_t) */
static size_t prime_cnt = 0;          /* number of primes stored */
static uint64_t prime_limit = 0;      /* upper limit that has been sieved */

static void recompute_primes(uint64_t limit)
{
    /* Allocate composite marker array. */
    unsigned char *is_comp = (unsigned char*)calloc((size_t)(limit + 1), 1);
    if (!is_comp) abort();

    uint64_t sq = (uint64_t)floor(sqrt((double)limit));
    for (uint64_t i = 2; i <= sq; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* Count primes. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Allocate primes array. */
    idx_t *new_primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_primes) abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) new_primes[idx++] = (idx_t)i;

    free(primes);
    free(is_comp);
    primes = new_primes;
    prime_cnt = cnt;
    prime_limit = limit;
}

static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;   /* already have enough primes */

    uint64_t required = upper_bound_nth_prime((idx_t)n);
    /* If the required limit is already covered, just rebuild. */
    if (required > prime_limit)
        recompute_primes(required);
    else if (prime_cnt < (size_t)n)
        recompute_primes(required);
}

/* ------------------------------------------------------------------ */
/* Cache for transformed primes. (Fixed size: 200 000 entries)        */
/* ------------------------------------------------------------------ */
#define CACHE_CAP 200000
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 */

    /* If the requested index is within the cache size, use it. */
    if (n <= CACHE_CAP)
    {
        if (n > cache_len)            /* need to extend cache */
        {
            ensure_primes(n);
            for (size_t i = cache_len; i < (size_t)n; ++i)
                cache[i] = transform_digits((seq_t)primes[i]);
            cache_len = (size_t)n;
        }
        return cache[n - 1];
    }

    /* For indices beyond the cache, compute on the fly. */
    ensure_primes(n);
    return transform_digits((seq_t)primes[n - 1]);
}
