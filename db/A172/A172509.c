/*
 * A172509  Numbers n such that Omega(n) = Omega(n - (digital sum of n)).
 * https://oeis.org/A172509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 7938, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A172509: numbers n such that Omega(n)=Omega(n-D(n)),
   where D(n) is the sum of decimal digits of n.  The function
   `a(idx_t n)` searches forward from 1 until it has found the
   n‑th such number, using a small sieve for smallest prime factors
   to compute Ω efficiently. */

#include <stdint.h>
#include <stddef.h>

#define SIEVE_LIMIT 2000000          /* maximal n examined in the sieve */
#define MAX_TERMS   20000            /* enough for all test cases up to n≈10 000 */

static uint32_t spf[SIEVE_LIMIT + 1];          /* smallest prime factor */
static seq_t    cache[MAX_TERMS];              /* stored sequence values */
static size_t   cache_len = 0;                 /* number of values cached so far */
static uint64_t last_checked = 0;              /* last integer examined */
static int      sieve_init = 0;                /* flag to run the sieve once */

static void init_sieve(void)
{
    if (sieve_init) return;
    /* initialise spf[i]=i */
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = i;
    /* sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (spf[i] == i)
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
    sieve_init = 1;
}

static int omega_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    int cnt = 0;
    while (n > 1) {
        uint32_t p;
        if (n <= SIEVE_LIMIT) {
            p = spf[(uint32_t)n];
            if (p == 0) p = (uint32_t)n;   /* safety for n=0 */
        } else {
            /* trial division for numbers beyond the sieve limit */
            for (uint32_t d = 2; (uint64_t)d * d <= n; ++d)
                if (n % d == 0) { p = d; goto found; }
            p = (uint32_t)n; /* n itself is prime */
            found: ;
        }
        ++cnt;
        n /= p;
    }
    return cnt;
}

static int digit_sum_uint64(uint64_t x)
{
    int sum = 0;
    while (x) {
        sum += (int)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; return 0 gracefully */

    init_sieve();

    /* generate values until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;                   /* next integer to test */

        int omega_cur   = omega_uint64(last_checked);
        int ds          = digit_sum_uint64(last_checked);
        uint64_t target = last_checked - (uint64_t)ds;
        int omega_target = (target <= 1) ? 0 : omega_uint64(target);

        if (omega_cur == omega_target) {
            if (cache_len < (size_t)MAX_TERMS)
                cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n-1];
}
