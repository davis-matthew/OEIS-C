/*
 * A230514  Number of ways to write n = a + b + c (0 < a <= b <= c) such that
 *          all the three numbers a*(a+1)-1, b*(b+1)-1, c*(c+1)-1 are Sophie
 *          Germain primes.
 * https://oeis.org/A230514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3616, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: count triples a+b+c=n (0<a<=b<=c) with a*(a+1)-1, b*(b+1)-1, c*(c+1)-1 all Sophie Germain primes */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------  Prime test – deterministic Miller‑Rabin for 64‑bit ---------- */
static uint64_t mod_pow_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    unsigned __int128 result = 1;
    unsigned __int128 base   = a % mod;
    while (d > 0) {
        if (d & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        d >>= 1;
    }
    return (uint64_t)result;
}
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i=0;i<sizeof(small_primes)/sizeof(small_primes[0]);++i){
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i=0;i<sizeof(bases)/sizeof(bases[0]);++i){
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a,d,n);
        if (x==1 || x==n-1) continue;
        bool comp = true;
        for (int r=1;r<s;r++){
            x = (unsigned __int128)x * x % n;
            if (x == n-1){ comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}
/* ----------  Sophie‑Germain check ---------- */
static bool sg_primality(uint64_t a)
{
    uint64_t p = a * (a + 1) - 1;
    if (!is_prime_uint64(p)) return false;
    uint64_t q = 2 * p + 1;
    return is_prime_uint64(q);
}

/* ----------  Static state ---------- */
static seq_t *SEQ          = NULL;   /* sequence values, SEQ[0] unused */
static idx_t  SEQ_SIZE    = 0;       /* current maximum computed index */
static bool  *IS_SG       = NULL;   /* 0/1 array, IS_SG[i] == true if i is SG */
static idx_t  SG_LIMIT    = 0;       /* maximum number for which IS_SG is valid */

static idx_t *SG_LIST     = NULL;   /* compact list of SG numbers */
static idx_t  SG_COUNT    = 0;      /* number of SG numbers in SG_LIST */

/* ----------  Helper to compute IS_SG up to a given limit ---------- */
static void ensure_is_sg_limit(idx_t limit)
{
    if (limit <= SG_LIMIT) return;
    bool *new_arr = (bool*)realloc(IS_SG, (size_t)(limit + 1));
    if (!new_arr) return; /* allocation failure – silently ignore */
    for (idx_t i = SG_LIMIT + 1; i <= limit; ++i) {
        new_arr[i] = sg_primality((uint64_t)i);
    }
    new_arr[0] = false;   /* a = 0 never used */
    IS_SG = new_arr;
    SG_LIMIT = limit;
}

/* ----------  Build list of SG numbers up to a given limit ---------- */
static void rebuild_sg_list(idx_t limit)
{
    ensure_is_sg_limit(limit);
    SG_COUNT = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (IS_SG[i]) ++SG_COUNT;
    SG_LIST = (idx_t*)realloc(SG_LIST, (size_t)SG_COUNT * sizeof(idx_t));
    idx_t pos = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (IS_SG[i]) SG_LIST[pos++] = i;
}

/* ----------  Compute all a(n) up to n ---------- */
static void compute_up_to(idx_t n)
{
    if (n <= SEQ_SIZE) return;
    SEQ = (seq_t*)realloc(SEQ, (size_t)(n + 1) * sizeof(seq_t));
    if (!SEQ) return; /* ignore failure */
    /* initialise new part of array to zero */
    for (idx_t i = SEQ_SIZE + 1; i <= n; ++i) SEQ[i] = 0;

    rebuild_sg_list(n);

    /* iterate over all triples of SG numbers (a<=b<=c) with sum ≤ n */
    for (size_t ia = 0; ia < SG_COUNT; ++ia) {
        idx_t a = SG_LIST[ia];
        if (a > n / 3) break;                /* minimal possible sum too large */
        for (size_t ib = ia; ib < SG_COUNT; ++ib) {
            idx_t b = SG_LIST[ib];
            if (a + 2*b > n) break;          /* minimal sum with c=b already too large */
            for (size_t ic = ib; ic < SG_COUNT; ++ic) {
                idx_t c = SG_LIST[ic];
                idx_t sum = a + b + c;
                if (sum > n) break;          /* further c are larger */
                SEQ[sum] = SEQ[sum] + (seq_t)1;
            }
        }
    }
    SEQ_SIZE = n;
}

/* ----------  Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* for indices below offset */
    compute_up_to(n);
    return SEQ[n];
}
