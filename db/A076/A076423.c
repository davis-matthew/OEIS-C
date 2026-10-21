/*
 * A076423  Number of iterations of the mapping k ->
 *          abs(reverse(lpd(k))-reverse(gpf(k))) to reach zero, or -1 if zero
 *          is never reached. lpd(k) is the largest proper divisor and gpf(k)
 *          is the greatest prime factor of k.
 * https://oeis.org/A076423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of iterations of k → |rev(lpd(k)) – rev(gpf(k))|
 * until k reaches 0, or –1 if it never reaches 0.
 *
 * lpd(k) = largest proper divisor of k
 * gpf(k) = greatest prime factor of k
 *
 * Implementation uses a simple incremental search with a dynamic
 * vector of previously seen values to detect cycles.  All arithmetic
 * is done in 64‑bit unsigned integers; the sequence values for n ≤ 10000
 * are comfortably within this range.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
 * Simple prime sieve (up to 1 000 000) – used for factorisation
 * ------------------------------------------------------------------ */
static bool primes_init = false;
static uint32_t *prime_tbl = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_init) return;
    const uint32_t MAXP = 1000000;
    bool *sieve = (bool *)calloc(MAXP + 1, sizeof(bool));
    for (uint32_t i = 2; i * i <= MAXP; ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= MAXP; j += i)
                sieve[j] = true;
    for (uint32_t i = 2; i <= MAXP; ++i)
        if (!sieve[i]) ++prime_cnt;
    prime_tbl = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (uint32_t i = 2; i <= MAXP; ++i)
        if (!sieve[i]) prime_tbl[pos++] = i;
    free(sieve);
    primes_init = true;
}

/* ------------------------------------------------------------------
 * Helper: reverse decimal digits of a 64‑bit integer
 * ------------------------------------------------------------------ */
static uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

/* ------------------------------------------------------------------
 * Helper: compute largest proper divisor and greatest prime factor
 * ------------------------------------------------------------------ */
static void lpd_gpf(uint64_t n, uint64_t *lpd, uint64_t *gpf)
{
    if (n <= 1) { *lpd = 1; *gpf = 1; return; }
    init_primes();
    uint64_t spf = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * prime_tbl[i] <= temp; ++i) {
        uint32_t p = prime_tbl[i];
        if (temp % p == 0) { spf = p; break; }
    }
    if (spf == 0) {               /* n is prime */
        *lpd = 1;
        *gpf = n;
        return;
    }
    *lpd = n / spf;               /* largest proper divisor */

    /* greatest prime factor */
    uint64_t maxpf = spf;
    temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * prime_tbl[i] <= temp; ++i) {
        uint32_t p = prime_tbl[i];
        if (temp % p == 0) {
            maxpf = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) maxpf = temp;
    *gpf = maxpf;
}

/* ------------------------------------------------------------------
 * Helper: next value of the mapping
 * ------------------------------------------------------------------ */
static uint64_t next_val(uint64_t k)
{
    uint64_t lpd, gpf;
    lpd_gpf(k, &lpd, &gpf);
    uint64_t rl = rev_uint64(lpd);
    uint64_t rg = rev_uint64(gpf);
    return (rl > rg) ? rl - rg : rg - rl;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)-1;           /* outside offset */

    uint64_t k = n;
    seq_t steps = 0;

    /* dynamic array for previously seen values */
    size_t seen_cap = 16;
    seq_t *seen = (seq_t *)malloc(seen_cap * sizeof(seq_t));
    size_t seen_len = 0;

    while (1) {
        if (k == 0) {                    /* reached zero */
            free(seen);
            return steps;
        }
        /* check for cycle */
        for (size_t i = 0; i < seen_len; ++i)
            if (seen[i] == k) {          /* cycle detected – never reaches 0 */
                free(seen);
                return (seq_t)-1;
            }
        /* store current value */
        if (seen_len == seen_cap) {
            seen_cap <<= 1;
            seen = (seq_t *)realloc(seen, seen_cap * sizeof(seq_t));
        }
        seen[seen_len++] = k;

        /* advance to next value */
        k = next_val(k);
        ++steps;
    }
}
