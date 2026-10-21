/*
 * A073609  a(0) = 2; a(n) for n > 0 is the smallest prime greater than a(n-1)
 *          that differs from a(n-1) by a square.
 * https://oeis.org/A073609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 124, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2; for n > 0, a(n) is the next prime after a(n-1)
   such that (a(n) – a(n-1)) is a perfect square.  The sequence is
   generated lazily with a static cache.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- helper: 64‑bit modular multiplication --------- */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t m)
{
    __uint128_t r = (__uint128_t)a * b;
    return (uint64_t)(r % m);
}

/* --------- helper: modular exponentiation (uint64_t) --------- */
static uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    a %= m;
    while (e) {
        if (e & 1) r = mul_mod64(r, a, m);
        a = mul_mod64(a, a, m);
        e >>= 1;
    }
    return r;
}

/* --------- helper: deterministic Miller–Rabin for 64‑bit --------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i=0;i<sizeof(small)/sizeof(small[0]);++i){
        if (n==small[i]) return true;
        if (n%small[i]==0) return false;
    }
    uint64_t d=n-1,s=0;
    while ((d&1)==0){ d>>=1; ++s; }
    static const uint64_t witnesses[] = {2,3,5,7,11,13,17};
    for (size_t i=0;i<sizeof(witnesses)/sizeof(witnesses[0]);++i){
        uint64_t a = witnesses[i]%n;
        if (a==0) continue;
        uint64_t x = pow_mod64(a,d,n);
        if (x==1 || x==n-1) continue;
        bool comp=true;
        for (uint64_t r=1;r<s;++r){
            x=mul_mod64(x,x,n);
            if (x==n-1){ comp=false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* --------- helper: integer sqrt (uint64_t) --------- */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo=0,hi=1ULL<<32;
    while (lo<hi){
        uint64_t mid=(lo+hi)>>1;
        if (mid*mid<=x) lo=mid+1; else hi=mid;
    }
    return lo-1;
}

/* --------- cache management --------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static void ensure_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap?cache_cap*2:16;
    while (new_cap < need) new_cap*=2;
    cache = (seq_t*)realloc(cache,new_cap*sizeof(seq_t));
    cache_cap = new_cap;
}

/* --------- main routine --------- */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        ensure_cap(1);
        cache[0] = (seq_t)2ULL;
        cache_len = 1;
    }

    if (n < 0) return (seq_t)0;            /* out of range guard */
    if ((size_t)n < cache_len) return cache[n];

    /* build sequence up to n */
    ensure_cap((size_t)n+1);
    for (size_t i=cache_len;i<= (size_t)n;++i) {
        seq_t prev = cache[i-1];
        uint64_t prev_u = (uint64_t)prev;   /* assumes prev fits in 64‑bit */
        uint64_t cand = prev_u + 1;
        if ((cand & 1) == 0) cand++;        /* start with odd candidate */
        seq_t next = 0;
        while (true) {
            if (is_prime_uint64(cand)) {
                uint64_t diff = cand - prev_u;
                uint64_t r = isqrt_uint64(diff);
                if (r*r == diff) { next = (seq_t)cand; break; }
            }
            cand += 2;
            if (cand == 2) cand = 3;        /* wrap for low numbers (unnecessary) */
        }
        cache[i] = next;
    }
    cache_len = n+1;
    return cache[n];
}
