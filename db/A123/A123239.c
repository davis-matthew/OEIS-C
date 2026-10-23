/*
 * A123239  Primes that do not divide 3^k - 2 for any k.
 * https://oeis.org/A123239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 2774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123239: primes that do not divide 3^k - 2 for any k.
   We decide for each prime p whether there is an integer k such that
   3^k ≡ 2 (mod p).  If no such k exists, p is included in the
   sequence.  The test is performed by a baby‑step giant‑step (BSGS)
   discrete‑log search.  The search stops as soon as a solution is
   found and the prime is rejected.  The algorithm is O(√p) per prime,
   which is fast enough for the required range.  The sequence is
   memoised so that calls with arbitrary n are answered in order
   independent of the call order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* Structure used for the BSGS table */
struct pair {
    uint64_t v;
    idx_t   j;
};

static int cmp_pair(const void *p1, const void *p2)
{
    const struct pair *a = (const struct pair *)p1;
    const struct pair *b = (const struct pair *)p2;
    if (a->v < b->v) return -1;
    if (a->v > b->v) return 1;
    return 0;
}

/* Test whether there exists k such that 3^k ≡ 2 (mod p). */
static bool has_k_solution(uint64_t p)
{
    if (p == 2 || p == 3)      /* 2 and 3 are always in the sequence */
        return false;

    /* Determine m = ceil(sqrt(p-1))  */
    idx_t m = 0;
    while ((m + 1) * (m + 1) <= p - 1)
        ++m;
    ++m;                           /* ceil */

    /* Prepare the baby‑step table: 3^j for j = 0 .. m-1  */
    static struct pair tbl[1024]; /* m will never exceed 1024 for the required range */
    uint64_t val = 1;
    tbl[0].v = 1;
    tbl[0].j = 0;
    for (idx_t j = 1; j < m; ++j) {
        val = (unsigned __int128)val * 3 % p;
        tbl[j].v = val;
        tbl[j].j = j;
    }
    qsort(tbl, (size_t)m, sizeof(struct pair), cmp_pair);

    /* Compute factor = 3^{-m} mod p  */
    uint64_t factor = mod_pow_uint64(3, (p - 1) - m, p);

    /* Giant‑step loop: search for 2 * (3^{-m})^i in the table  */
    uint64_t cur = 2 % p;
    for (idx_t i = 0; i < m; ++i) {
        /* binary search of cur in tbl */
        idx_t lo = 0, hi = (idx_t)m - 1;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            if (tbl[mid].v == cur)
                return true;   /* solution found */
            if (tbl[mid].v < cur)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        cur = (unsigned __int128)cur * factor % p;
    }
    return false;              /* no solution found */
}

/* ---------- Prime generation utilities ---------- */
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static uint64_t next_candidate = 5;   /* next odd number to test */

static void ensure_prime_capacity(void)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = new_cap;
    }
}

static void add_prime(uint64_t p)
{
    ensure_prime_capacity();
    primes[primes_len++] = p;
}

static bool isprime_trial(uint64_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t q = primes[i];
        if (q * q > n)
            break;
        if (n % q == 0)
            return false;
    }
    return true;
}

static uint64_t get_next_prime(void)
{
    if (primes_len == 0) {
        add_prime(2);
        return 2;
    }
    if (primes_len == 1) {
        add_prime(3);
        return 3;
    }
    while (true) {
        if (isprime_trial(next_candidate)) {
            add_prime(next_candidate);
            next_candidate += 2;
            return primes[primes_len - 1];
        }
        next_candidate += 2;
    }
}

/* ---------- Memoisation array for the sequence ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- The required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* 0‑based index not used, defined to avoid UB */

    while (cache_len < (size_t)n) {
        uint64_t p = get_next_prime();    /* next prime to test */
        bool ok = !has_k_solution(p);
        if (ok)
            cache_append((seq_t)p);
    }
    return cache[n - 1];
}
