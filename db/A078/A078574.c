/*
 * A078574  Number of divisors of the average of n-th twin prime pair.
 * https://oeis.org/A078574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005( A014574(n) ) i.e. the number of divisors of the
 *            average of the n‑th twin prime pair
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Check whether a 64‑bit integer is prime using trial division. */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Count the positive divisors of a 64‑bit integer. */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t result = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            do { n /= p; ++exp; } while (n % p == 0);
            result *= (exp + 1);
        }
    }
    if (n > 1) result *= 2;          /* remaining prime factor */
    return result;
}

/* Dynamically stored list of lower primes in twin prime pairs. */
static idx_t *twinLow      = NULL;   /* lower primes */
static size_t twinLow_len   = 0;      /* how many stored */
static size_t twinLow_cap   = 0;      /* allocated capacity */
static uint64_t last_checked = 1;     /* last odd number examined */

/* Grow the twinLow array to accommodate at least minSize elements. */
static void ensure_twin_primes(size_t minSize)
{
    while (twinLow_len < minSize) {
        /* Find next odd candidate and test for a twin prime pair. */
        do { ++last_checked; } while (last_checked % 2 == 0);   /* keep odd */

        if (is_prime(last_checked) && is_prime(last_checked + 2)) {
            /* Store the lower prime of the twin pair. */
            if (twinLow_len == twinLow_cap) {
                size_t new_cap = twinLow_cap ? twinLow_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(twinLow, new_cap * sizeof(idx_t));
                if (!tmp) abort();   /* out of memory */
                twinLow      = tmp;
                twinLow_cap  = new_cap;
            }
            twinLow[twinLow_len++] = (idx_t)last_checked;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1; safe return */

    ensure_twin_primes((size_t)n);

    uint64_t avg = (uint64_t)twinLow[n - 1] + 1;   /* average of the pair */
    return (seq_t)divisor_count(avg);
}
