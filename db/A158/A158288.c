/*
 * A158288  The number of twin prime pairs in the n-th century.
 * https://oeis.org/A158288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of twin‑prime first members in the n‑th century
 *            i.e. the count of primes p ≤ 100*n−3 with p≥100*n−99 and p+2
 *            prime. It is implemented by a growing prime sieve and a prefix
 *            array of such primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- include needed headers --------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- integer square root (floor) ---------------------------------------- */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* --- global sieve and prefix -------------------------------------------- */
static bool *is_prime = NULL;      /* size: current_limit + 3               */
static seq_t *twin_prefix = NULL;  /* size: current_limit + 1               */
static idx_t current_limit = 0;    /* maximum prime value already sieved     */

/* --- grow sieve and prefix up to at least 'limit' (≥0) ----------------- */
static void _grow_sieve(idx_t limit)
{
    if (limit < current_limit)
        return;                      /* already up to date */

    /* Extend the sieve a bit: we need to test p+2, so allocate +2 beyond limit */
    idx_t new_limit = limit;
    size_t prime_size = (size_t)new_limit + 3;      /* indices 0 .. new_limit+2 */
    size_t prefix_size = (size_t)new_limit + 1;     /* indices 0 .. new_limit */

    bool *new_is_prime = (bool *)calloc(prime_size, 1);
    if (!new_is_prime) return;               /* memory error – ignore */
    for (size_t i = 2; i < prime_size; ++i)
        new_is_prime[i] = true;

    idx_t sqrt_limit = _isqrt_u64((idx_t)prime_size - 1);
    for (idx_t i = 2; i <= sqrt_limit; ++i) {
        if (new_is_prime[i]) {
            for (idx_t j = i * i; j < prime_size; j += i)
                new_is_prime[j] = false;
        }
    }

    seq_t *new_prefix = (seq_t *)calloc(prefix_size, sizeof(seq_t));
    if (!new_prefix) {
        free(new_is_prime);
        return;                               /* memory error – ignore */
    }

    seq_t count = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (new_is_prime[i] && new_is_prime[i + 2])
            ++count;
        new_prefix[i] = count;
    }
    new_prefix[0] = 0;
    new_prefix[1] = 0;

    free(is_prime);
    free(twin_prefix);

    is_prime = new_is_prime;
    twin_prefix = new_prefix;
    current_limit = new_limit;
}

/* --- Sequence A158288 ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* below offset – undefined behaviour allowed */
        return 0;

    /* n-th century: [100*n-99 , 100*n-3] */
    idx_t upper = 100 * n - 3;   /* highest first member allowed */
    idx_t lower = 100 * n - 99;  /* lowest first member allowed */

    if (upper < lower)         /* empty interval, safe guard */
        return 0;

    _grow_sieve(upper);                 /* ensure data up to 'upper' */

    /* Count twin‑prime first members in [lower , upper] */
    idx_t lo = lower;
    idx_t hi = upper;

    /* Ensure indices are within bounds of the prefix array */
    if (lo < 0)
        lo = 0;
    if (hi > current_limit)
        hi = current_limit;

    seq_t result = twin_prefix[hi];
    if (lo > 0)
        result -= twin_prefix[lo - 1];
    return result;
}
