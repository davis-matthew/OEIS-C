/*
 * A164023  Smallest of largest parts in partitions of n into exactly three
 *          primes.
 * https://oeis.org/A164023
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 286, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for OEIS A164023: smallest of largest part in partitions of n into exactly three primes */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Global prime cache: highest n for which primes are generated */
static idx_t   _prime_limit = 0;     /* highest n for which primes are generated */
static bool   *_prime_is    = NULL;  /* primality flags up to _prime_limit */
static seq_t  *_prime_list  = NULL;  /* list of primes up to _prime_limit */
static size_t  _prime_cnt   = 0;     /* number of primes stored */

/* Ensure that primes up to 'limit' are available (simple sieve) */
static void _ensure_primes(idx_t limit)
{
    if (limit <= _prime_limit) return;

    /* Allocate and sieve */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) abort();                /* memory error */

    /* 0 and 1 are not prime */
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    /* initialise all as true, except 0,1 */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes and build list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    seq_t *list = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!list) abort();
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) list[pos++] = (seq_t)i;

    /* replace old data */
    free(_prime_is);
    free(_prime_list);
    _prime_is   = is_prime;
    _prime_list = list;
    _prime_cnt  = cnt;
    _prime_limit = limit;
}

/* binary search: first index of prime >= value */
static size_t _lower_bound_prime(seq_t value)
{
    size_t lo = 0, hi = _prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (_prime_list[mid] < value) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* below offset: behaviour undefined, return 0 */

    _ensure_primes(n);            /* primes up to n are ready */

    /* m must be prime ≥ ceil(n/3) and ≤ n-4 (since other two primes ≥2) */
    seq_t start = (seq_t)(n / 3);
    if (n % 3) ++start;          /* ceil */

    size_t idx = _lower_bound_prime(start);   /* first prime ≥ start */

    for (; idx < _prime_cnt; ++idx) {
        seq_t m = _prime_list[idx];
        if (m > (seq_t)(n - 4)) break;    /* cannot be largest part */

        seq_t target = (seq_t)n - m;    /* sum of the other two primes */

        if (target < (seq_t)4) continue;           /* two primes need at least 2+2 */
        if (target > (seq_t)2 * m) continue;       /* each ≤ m, so sum ≤ 2*m */

        /* two‑pointer search for p+q = target with p,q ≤ m */
        size_t left  = 0;
        size_t right = idx;                 /* indices of primes ≤ m */
        while (left <= right) {
            seq_t sum = _prime_list[left] + _prime_list[right];
            if (sum == target) return m;  /* found a valid partition */
            if (sum < target) ++left;
            else /* sum > target */ {
                if (right == 0) break;    /* avoid underflow */
                --right;
            }
        }
    }

    /* Should never happen for n ≥ 6 by the Goldbach‑type theorem */
    return (seq_t)n;
}
