/*
 * A137263  Interprimes (A024675) == 2 (mod 3).
 * https://oeis.org/A137263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer that is the midpoint of two consecutive
 *            * primes and congruent to 2 (mod 3). Computed by sieving primes
 *            up to a * fixed limit and collecting those midpoints. The first
 *            term is 26
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* static storage for computed values */
static int    _a137263_initialized = 0;
static seq_t * _a137263_vals = NULL;
static size_t  _a137263_len  = 0;

/* helper: sieve primes up to LIMIT and return array, count */
static void _sieve_primes(idx_t LIMIT, uint64_t **primes_out, size_t *cnt_out)
{
    /* sieve of Eratosthenes (simple byte array) */
    uint8_t *sieve = (uint8_t*)calloc((size_t)LIMIT + 1, 1);
    if (!sieve) abort();
    sieve[0] = sieve[1] = 1;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;
        }
    }

    /* count primes */
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            ++primes_cnt;

    /* collect primes */
    uint64_t *primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) abort();
    size_t p = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            primes[p++] = (uint64_t)i;

    free(sieve);
    *primes_out = primes;
    *cnt_out    = primes_cnt;
}

/* initialize static data once */
static void _a137263_init(void)
{
    if (_a137263_initialized) return;

    /* Upper bound that yields far more than 10 000 terms */
    const idx_t LIMIT = 5000000ULL;   /* 5 million */

    uint64_t * primes   = NULL;
    size_t    primes_cnt = 0;
    _sieve_primes(LIMIT, &primes, &primes_cnt);

    /* Allocate space for all possible midpoints (≤ primes_cnt) */
    seq_t *vals = (seq_t*)malloc(primes_cnt * sizeof(seq_t));
    if (!vals) abort();
    size_t v = 0;

    /* Iterate over consecutive prime pairs.
     * Skip the first pair (2,3) because its sum is odd, and the
     * midpoint would be truncated.  All other pairs are odd + odd = even. */
    for (size_t i = 1; i + 1 < primes_cnt; ++i) {
        seq_t mid = ((seq_t)primes[i] + (seq_t)primes[i + 1]) / 2;
        if ((mid % 3) == 2)      /* keep only numbers ≡ 2 (mod 3) */
            vals[v++] = mid;
    }

    /* Shrink to actual size */
    seq_t *tmp = (seq_t*)realloc(vals, v * sizeof(seq_t));
    if (tmp) vals = tmp;

    /* Free primes array */
    free(primes);

    _a137263_vals = vals;
    _a137263_len  = v;
    _a137263_initialized = 1;
}

/* public routine required by the specification */
seq_t a(idx_t n)
{
    _a137263_init();
    if (n == 0 || n > _a137263_len) return 0; /* out of range */
    return _a137263_vals[n - 1];
}
