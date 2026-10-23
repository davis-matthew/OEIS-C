/*
 * A108184  a(n) = smallest prime such that a(n) + 2n is also prime and such
 *          that a(n) > a(n-1).
 * https://oeis.org/A108184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime p > a(n-1) such that p + 2*n is also prime
 *            (with a(0) = the first prime = 2). The sequence is computed
 *            lazily and memoised, so any order of calls works
 * Range:     u8 n <= 21, u16 n <= 1230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 64‑bit deterministic Miller--Rabin primality test ----------------------------------- */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}
static inline uint64_t modpow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = modmul64(r, a, m);
        a = modmul64(a, a, m);
        e >>= 1;
    }
    return r;
}
static inline bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t j = 0; j < sizeof(small)/sizeof(small[0]); ++j)
    {
        if (n == small[j]) return true;
        if (n % small[j] == 0) return false;
    }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t j = 0; j < sizeof(bases)/sizeof(bases[0]); ++j)
    {
        uint64_t a = bases[j] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = modmul64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* --------------- Memoised storage for the sequence ------------------------------ */
static seq_t *seq   = NULL;   /* array of computed values */
static idx_t  len   = 0;      /* number of values stored */
static size_t cap   = 0;      /* current capacity of seq */

/* --------------- Helper: grow the storage to accommodate index n ----------------- */
static void ensure_capacity(idx_t n)
{
    if (len > n) return;
    /* grow capacity if needed */
    if (cap == 0) cap = 1;
    while (cap <= (size_t)n) cap <<= 1;
    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    /* len remains its old value; unused slots are uninitialized */
}

/* ----------------------- Main routine ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < len) return seq[n];   /* already known */

    ensure_capacity(n);

    /* start from the previous term (or 0 if none) and find successive terms */
    seq_t prev = (len > 0) ? seq[len - 1] : (seq_t)0;

    for (idx_t i = len; i <= n; ++i)
    {
        /* candidate must be > prev; start at max(prev+1,2) and skip evens */
        uint64_t cand = (uint64_t)(prev + 1);
        if (cand < 2ULL) cand = 2ULL;
        if ((cand & 1ULL) == 0ULL && cand > 2ULL) cand++;   /* make odd unless 2 */

        /* search for the next prime satisfying the condition */
        while (true)
        {
            if (isPrime64(cand) && isPrime64(cand + 2ULL * (uint64_t)i))
                break;            /* found it */
            cand += 2ULL;          /* skip even numbers */
        }
        seq[i] = (seq_t)cand;
        prev   = seq[i];
    }
    len = n + 1;
    return seq[n];
}
