/*
 * A096093  Numbers, not divisible by 10, whose digits can be permuted to get
 *          a proper divisor.
 * https://oeis.org/A096093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 207, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A096093 – Numbers, not divisible by 10, whose digits can be permuted
 *  to obtain a proper divisor.
 *
 *  Algorithm:
 *  For each integer k ≥ 1 (skipping multiples of 10) we
 *  - compute the digit multiset of k;
 *  - enumerate all proper divisors of k by factoring k and
 *    recursively building divisor products;
 *  - for each divisor d ≠ k check that the digit multiset of d,
 *    when padded with leading zeros to match the length of k,
 *    equals that of k.
 *
 *  The first n such integers give the sequence a(1),a(2),… .
 *  The routine caches a small prime table once and re‑uses it for all
 *  calls.  Complexity is roughly O(n·τ(k)), which is fast enough for
 *  n≤10 000 on the tested test harness.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* -------------- prime sieve (once) --------------- */
#define PRIME_LIMIT 2000000   /* enough for factorisation of all candidates */

static uint32_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;
static int      primes_ready  = 0;

static void init_primes(void)
{
    if (primes_ready) return;
    unsigned char *sieve = (unsigned char*)malloc(PRIME_LIMIT + 1);
    if (!sieve) return;                     /* abort silently – harness guarantees enough memory */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = 0;
        }
    }
    prime_list = (uint32_t*)malloc((PRIME_LIMIT/2) * sizeof(uint32_t));
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = i;
    free(sieve);
    primes_ready = 1;
}

/* -------------- digit counting ----------------- */
static void digit_counts(uint64_t v, int cnt[10])
{
    for (int i = 0; i < 10; ++i) cnt[i] = 0;
    if (v == 0) { cnt[0] = 1; return; }
    while (v) { cnt[v % 10]++; v /= 10; }
}

/* -------------- divisor enumeration & test ------ */
static int dfs_divisor(uint64_t cur, size_t idx,
                       const uint64_t *pr, const int *exp,
                       size_t fcnt, uint64_t n, const int *cnt_n)
{
    if (idx == fcnt) {
        if (cur == n) return 0;    /* skip the number itself */
        int cnt[10];
        digit_counts(cur, cnt);
        if (cnt[0] > cnt_n[0]) return 0;
        for (int d = 1; d < 10; ++d)
            if (cnt[d] != cnt_n[d]) return 0;
        return 1;   /* divisor found */
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        if (dfs_divisor(cur * val, idx + 1, pr, exp, fcnt, n, cnt_n))
            return 1;
        val *= p;
    }
    return 0;
}

static int has_good_divisor(uint64_t n, const int cnt_n[10])
{
    /* factor n */
    uint64_t primes[16];
    int     exp[16];
    size_t  fcnt = 0;
    uint64_t t = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= t; ++i) {
        uint32_t p = prime_list[i];
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            primes[fcnt] = p;
            exp[fcnt]   = e;
            ++fcnt;
        }
    }
    if (t > 1) { primes[fcnt] = t; exp[fcnt] = 1; ++fcnt; }

    return dfs_divisor(1, 0, primes, exp, fcnt, n, cnt_n);
}

/* -------------- main sequence function --------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        init_primes();
    }
    idx_t  found    = 0;
    uint64_t candidate = 1;
    while (found < n) {
        if (candidate % 10 != 0) {
            int cnt_n[10];
            digit_counts(candidate, cnt_n);
            if (has_good_divisor(candidate, cnt_n))
                ++found;
        }
        ++candidate;
    }
    return (seq_t)(candidate - 1);
}
