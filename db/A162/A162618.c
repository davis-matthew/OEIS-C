/*
 * A162618  Triangle read by rows in which row n lists n consecutive natural
 *          numbers A000027, starting with A008578(n-1) - n + 1.
 * https://oeis.org/A162618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = prime(r-1) - r + 1 + (n – tr(r-1) – 1) where r = smallest
 *            integer with r(r+1)/2 ≥ n and tr(k) = k(k+1)/2. For r = 1,
 *            prime(0) is taken as 1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------ */
/* Find the row r that contains the n-th element in the triangle */
/* ------------------------------------------------------------ */
static inline idx_t
row_of_idx(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;
    /* double high until it covers n */
    while (( (__uint128_t)high * (high + 1) / 2 ) < n)
        high <<= 1;
    /* binary search for minimal r with tr(r) >= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t val = ( (__uint128_t)mid * (mid + 1) ) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;                 /* r */
}

/* ------------------------------------------------------------ */
/* Prime number utilities                                       */
/* ------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;   /* 1‑indexed array of primes */
static size_t   prime_count  = 0;        /* number of primes stored */

/* Estimate an upper bound for the p‑th prime (p >= 1).  Uses Rosser's theorem */
static inline size_t
prime_upper_bound(size_t p)
{
    if (p < 6)
        return 15;
    double pd   = (double)p;
    double bound = pd * (log(pd) + log(log(pd)));
    return (size_t)(bound + 10.0);
}

/* Build or enlarge the list of primes to contain at least want primes */
static void
ensure_primes(size_t want)
{
    if (prime_count >= want)
        return;

    size_t limit = prime_upper_bound(want);
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime)
        return;                    /* out of memory – caller will fail */

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes in the sieve */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* Allocate or grow the prime list to hold the primes */
    uint64_t *new_list = (uint64_t *)realloc(prime_list,
                                            (cnt + 1) * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;                    /* allocation failed */
    }
    prime_list = new_list;
    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[prime_count++] = (uint64_t)i;

    free(is_prime);
}

/* Return the k‑th prime (k ≥ 1).  The first 'special' prime(0) is handled
   separately by the main a() function. */
static inline uint64_t
get_prime(size_t k)
{
    ensure_primes(k);
    return prime_list[k - 1];
}

/* ------------------------------------------------------------ */
/* Main OEIS sequence function                                */
/* ------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* Determine the row r that contains the n-th element. */
    idx_t r = row_of_idx(n);

    /* Compute how far into that row the sequence index n lies. */
    __uint128_t prev_tri = ( (__uint128_t)(r - 1) * r ) / 2;
    idx_t start_index = (idx_t)prev_tri + 1;              /* first index of row r */
    idx_t pos_in_row  = n - start_index;                  /* 0‑based offset within the row */

    /* Compute the value at the start of the row. */
    seq_t start;
    if (r == 1) {
        /* Row 1 starts with 1 by definition. */
        start = (seq_t)1;
    } else {
        uint64_t prime_val = get_prime((size_t)(r - 1));  /* A008578(r‑1) */
        start = (seq_t)prime_val - r + 1;
    }

    /* Return the element at position n. */
    return start + (seq_t)pos_in_row;
}
