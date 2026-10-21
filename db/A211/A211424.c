/*
 * A211424  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2+3*x*y=0.
 * https://oeis.org/A211424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with w,x,y∈{−n,…,0,…,n}
 *            and w^2 + 3xy = 0
 * Range:     u8 n <= 26, u16 n <= 3590, u32 n <= 9732, u64 n <= 9732,
 *            u128 n <= 9732, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ------------------------------------------------------------------------- */
/* Prime generation (once, up to 6000)                                        */
/* ------------------------------------------------------------------------- */
#define MAX_PRIME_SIEVE 6000

static uint32_t primes_arr[MAX_PRIME_SIEVE];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    bool sieve[MAX_PRIME_SIEVE + 1];
    for (size_t i = 0; i <= MAX_PRIME_SIEVE; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME_SIEVE; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME_SIEVE; j += i)
                sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= MAX_PRIME_SIEVE; ++i)
        if (sieve[i]) primes_arr[primes_cnt++] = (uint32_t)i;
    primes_inited = true;
}

/* ------------------------------------------------------------------------- */
/* Factorisation and divisor enumeration                                      */
/* ------------------------------------------------------------------------- */
static size_t count_divisors_in_bounds(uint64_t M, uint64_t n)
{
    /* factorise M */
    uint32_t facP[32];
    int facE[32];
    size_t facCnt = 0;
    uint64_t tmp = M;

    init_primes();
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes_arr[i] * primes_arr[i] <= tmp; ++i) {
        uint32_t p = primes_arr[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            facP[facCnt] = p;
            facE[facCnt] = e;
            ++facCnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        facP[facCnt] = (uint32_t)tmp;
        facE[facCnt] = 1;
        ++facCnt;
    }

    /* enumerate all (positive) divisors */
    uint64_t divis[2048];
    size_t divSize = 1;
    divis[0] = 1;

    for (size_t i = 0; i < facCnt; ++i) {
        uint64_t p = facP[i];
        int e = facE[i];
        size_t oldSize = divSize;
        uint64_t mult = 1;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < oldSize; ++j) {
                divis[divSize++] = divis[j] * mult;
            }
        }
    }

    /* count those satisfying d ≤ n and M/d ≤ n */
    size_t cnt = 0;
    for (size_t i = 0; i < divSize; ++i) {
        uint64_t d = divis[i];
        if (d <= n && M / d <= n) ++cnt;
    }
    return cnt;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function                                                     */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t total = 0;

    /* w must be a multiple of 3: w = 3*k, |w| ≤ n  */
    int64_t maxk = (int64_t)(n / 3);
    for (int64_t k = -maxk; k <= maxk; ++k) {
        if (k == 0) {
            total += (seq_t)4 * (seq_t)n + (seq_t)1;        /* 4n+1  */
        } else {
            uint64_t M = 3ULL * (uint64_t)k * (uint64_t)k;  /* 3*k^2  */
            size_t t = count_divisors_in_bounds(M, (uint64_t)n);
            total += (seq_t)2 * (seq_t)t;
        }
    }
    return total;
}
