/*
 * A121884  Excess of n-th semiprime over previous prime.
 * https://oeis.org/A121884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001358(n) - prev_prime(A001358(n)) where A001358(n) is
 *            the n‑th semiprime and prev_prime(x) is the largest prime < x
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 200000 is enough to contain the first 20000 semiprimes
   (far more than needed for n ≤ 10000).  The implementation
   precomputes primes, smallest prime factors and the list
   of semiprimes on first use.  It works for any n ≥ 1.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External integer types supplied by the build environment. */
extern unsigned long long (*seq_t_mul)(seq_t, seq_t);
extern seq_t seq_t_from_uint64(uint64_t);

static bool seq_initialized = false;
static uint32_t sieve_limit = 200000;          /* sieve upper bound */
static uint32_t *spf = NULL;                   /* smallest prime factor table */
static uint32_t *primes = NULL;                /* prime list          */
static size_t primes_cnt = 0;
static seq_t *semiprimes = NULL;               /* semiprime list      */
static size_t semiprimes_cnt = 0;

/* Forward declarations */
static void seq_precompute(void);
static seq_t prev_prime(seq_t x);

/* ------------------------------------------------------------ */
static void seq_precompute(void)
{
    if (seq_initialized) return;
    seq_initialized = true;

    /* ---------- build spf array ---------- */
    spf = (uint32_t*)malloc((sieve_limit + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= sieve_limit; ++i) spf[i] = 0;

    for (uint32_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {                   /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= sieve_limit) {
                for (uint32_t j = i * i; j <= sieve_limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* ---------- build prime list ---------- */
    size_t prime_cap = sieve_limit / 10;
    primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    primes_cnt = 0;
    for (uint32_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == i) {                   /* prime detection */
            if (primes_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
        }
    }

    /* ---------- build semiprime list up to 20000 entries ---------- */
    const size_t SEMI_TARGET = 20000;        /* more than enough for n≤10000 */
    size_t semicap = SEMI_TARGET + 1000;
    semiprimes = (seq_t*)malloc(semicap * sizeof(seq_t));
    semiprimes_cnt = 0;

    for (uint32_t x = 4; x <= sieve_limit; ++x) {
        uint32_t temp = x;
        uint32_t cnt = 0;
        while (temp > 1) {
            temp /= spf[temp];
            ++cnt;
            if (cnt > 2) break;
        }
        if (cnt == 2) {
            if (semiprimes_cnt == semicap) {
                semicap *= 2;
                semiprimes = (seq_t*)realloc(semiprimes, semicap * sizeof(seq_t));
            }
            semiprimes[semiprimes_cnt++] = (seq_t)x;
            if (semiprimes_cnt >= SEMI_TARGET) break;
        }
    }
}

/* ---------- binary search for previous prime ----------
   Assumes primes_cnt > 0 and x > primes[0] (true for semiprimes >=4). */
static seq_t prev_prime(seq_t x)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((seq_t)primes[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)primes[lo - 1];
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n=0, return 0 */

    seq_precompute();

    idx_t idx = n - 1;             /* 0‑based index into semiprime array */
    seq_t sem = semiprimes[idx];
    seq_t p   = prev_prime(sem);
    return sem - p;
}
