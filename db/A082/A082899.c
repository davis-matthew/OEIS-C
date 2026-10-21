/*
 * A082899  a(n) = A082893(n)-A000040(n), that is difference of n-th prime and
 *          number closest to it and divisible by n.
 * https://oeis.org/A082899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p[n] – n·⌊(⌊n/2⌋ + p[n])/n⌋ (p[n] = the n‑th prime)
 * Range:     i8 n <= 31, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes on demand and evaluates the formula using
   integer arithmetic only.  It works for seq_t signed of any width and
   for idx_t unsigned (uint64_t). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Static storage for the prime table. */
static uint64_t *prime_list   = NULL; /* array of primes */
static size_t   prime_count  = 0;      /* number of primes in the table */
static size_t   prime_cap    = 0;      /* allocated capacity */

/* Integer square root (floor) for 64‑bit unsigned integers. */
static uint64_t integer_sqrt(uint64_t x)
{
    uint64_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == x)  return mid;
        if (sq < x) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Ensure that at least n primes are stored in prime_list. */
static void ensure_prime_count(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    /* Grow the storage if needed. */
    if (prime_cap < (size_t)n) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();               /* allocation failure – terminate */
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Generate missing primes. */
    for (size_t i = prime_count; i < (size_t)n; ++i) {
        uint64_t candidate = (i == 0) ? 2 : prime_list[i - 1] + 1;
        for (;;) {
            unsigned int is_prime = 1;
            uint64_t limit = integer_sqrt(candidate);
            for (size_t j = 0; j < i && prime_list[j] <= limit; ++j) {
                if (candidate % prime_list[j] == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                prime_list[i] = candidate;
                break;
            }
            ++candidate;
        }
    }

    prime_count = (size_t)n;
}

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is defined for n≥1 */

    ensure_prime_count(n);

    seq_t   pn   = (seq_t)prime_list[n - 1];   /* p[n] */
    seq_t   nn   = (seq_t)n;
    seq_t   half = (seq_t)(n / 2);             /* floor(n/2) */
    seq_t   k    = (pn + half) / nn;          /* floor((pn+half)/n) */
    seq_t   m    = nn * k;                    /* nearest multiple of n */
    return m - pn;                            /* a(n) */
}
