/*
 * A185101  The number n written using the minimum number of terms in the base
 *          where the values of the places are 1 and primes (noncomposites).
 *          For multiple solutions the smallest binary value is chosen.
 * https://oeis.org/A185101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Algorithm for OEIS A185101:
 *
 *  Minimal binary vector (decimal representation) indicating a set of
 *  non‑composite summands (1 and primes) whose sum equals n.
 *  The set must be minimal in cardinality, and then the smallest
 *  lexicographic (decimal) binary string is chosen.
 *
 *  The construction follows the OEIS description:
 *      • n ≤ 6 – hard‑coded values
 *      • n prime – use the singleton {n}
 *      • n > 6, n‑2 prime – use {2, n‑2}
 *      • n > 6 even – use Goldbach pair (n/2‑k, n/2+k) with smallest k>0
 *      • n > 6 odd – iterate m = next prime > n/3, let even_part = n–m,
 *        find smallest k>0 such that (even_part/2‑k, even_part/2+k) are
 *        prime and distinct from m; the first feasible (m, p, q) gives the
 *        result.  If duplicates occur, advance m to the next prime.
 *
 *  The routine returns a seq_t value that is the decimal number formed by
 *  concatenating the binary vector bits (most significant bit first).
 *  The implementation is fully self‑contained and works for any
 *  integer types provided by the build environment.
 *
 *  Author: ChatGPT
 *  Date: 2026-10-23
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime sieve and helpers ---------- */
static bool *prime_sieve = NULL;          /* is_prime flag array, size = sieve_limit+1 */
static uint32_t *prime_list = NULL;       /* ascending list of primes */
static size_t prime_count = 0;
static idx_t sieve_limit = 0;

/* Build or rebuild the sieve up to 'limit' */
static void rebuild_sieve(idx_t limit)
{
    /* Allocate sieve array */
    bool *sieve = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!sieve) abort();
    for (size_t i = 0; i <= (size_t)limit; ++i) sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = false;
        }
    }

    /* Collect primes into list */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) ++cnt;

    uint32_t *list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!list) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) list[idx++] = (uint32_t)i;

    /* Replace old data */
    free(prime_sieve);
    free(prime_list);
    prime_sieve = sieve;
    prime_list = list;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Ensure sieve covers at least 'limit' */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;
    rebuild_sieve(limit);
}

/* Primality test using the sieve */
static bool is_prime(uint64_t v)
{
    if (v > (uint64_t)sieve_limit) ensure_sieve((idx_t)v);
    return prime_sieve[v];
}

/* Binary search for a prime's 1‑based index, return -1 if not prime */
static int prime_index(uint64_t val)
{
    if (val == 1) return 0;        /* special case for 1 */
    if (val > (uint64_t)sieve_limit) return -1;
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        uint32_t p = prime_list[mid];
        if (p < val) lo = mid + 1;
        else if (p > val) hi = mid;
        else return (int)(mid + 1); /* 1‑based */
    }
    return -1;
}

/* Next prime ≥ 'start' (if start is prime, returns that prime) */
static uint64_t next_prime(uint64_t start)
{
    if (start <= 2) return 2;
    if ((start & 1) == 0) ++start;
    while (true) {
        if (is_prime(start)) return start;
        start += 2;
    }
}

/* ---------- Encoding algorithm ---------- */
static seq_t encode_set(const uint64_t *vals, size_t count)
{
    /* Find maximum index among values */
    int max_idx = 0;
    int *idxs = (int*)malloc(count * sizeof(int));
    if (!idxs) abort();

    for (size_t i = 0; i < count; ++i) {
        uint64_t v = vals[i];
        int idx;
        if (v == 1) idx = 0;
        else {
            idx = prime_index(v);
            if (idx < 0) { free(idxs); abort(); } /* should not happen */
        }
        idxs[i] = idx;
        if (idx > max_idx) max_idx = idx;
    }

    /* Set bits */
    bool *bits = (bool*)calloc((size_t)max_idx + 1, sizeof(bool));
    if (!bits) abort();
    for (size_t i = 0; i < count; ++i) bits[idxs[i]] = true;

    /* Build decimal representation */
    seq_t result = 0;
    for (int i = max_idx; i >= 0; --i)
        result = result * 10 + (bits[i] ? 1 : 0);

    free(bits);
    free(idxs);
    return result;
}

/* ---------- Goldbach helper: minimal k for given mid ---------- */
static uint64_t minimal_k(uint64_t mid)
{
    for (uint64_t k = 1; ; ++k) {
        if (mid <= k) return 0;          /* no valid k, should not happen */
        if (is_prime(mid - k) && is_prime(mid + k))
            return k;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Manual values for n = 0 .. 6 */
    switch (n) {
    case 0: return 0;
    case 1: return 1;
    case 2: return 10;
    case 3: return 100;
    case 4: return 101;
    case 5: return 1000;
    case 6: return 1001;
    }

    /* Ensure associated primes are available */
    ensure_sieve(n);

    /* n is prime -> single-term representation */
    if (is_prime(n)) {
        uint64_t v[1] = { n };
        return encode_set(v, 1);
    }

    /* n > 6 and n‑2 prime -> {2, n‑2} */
    if (n > 6 && is_prime(n - 2)) {
        uint64_t v[2] = { 2, n - 2 };
        return encode_set(v, 2);
    }

    /* n > 6 even -> Goldbach pair */
    if (n > 6 && (n & 1) == 0) {
        uint64_t mid = n / 2;
        uint64_t k = minimal_k(mid);
        if (k == 0) return 0;          /* should never happen */
        uint64_t p = mid - k, q = mid + k;
        uint64_t v[2] = { p, q };
        return encode_set(v, 2);
    }

    /* n > 6 odd -> weak Goldbach triple */
    uint64_t m = next_prime(n / 3 + 1);    /* m > n/3 */
    seq_t best = 0;
    bool found = false;

    while (m <= n) {
        uint64_t even_part = n - m;
        if ((even_part & 1) == 0 && even_part >= 2) {
            uint64_t mid = even_part / 2;
            uint64_t k = minimal_k(mid);
            if (k != 0) {
                uint64_t p = mid - k, q = mid + k;
                if (p != m && q != m) {          /* distinctness check */
                    uint64_t v[3] = { m, p, q };
                    seq_t cand = encode_set(v, 3);
                    if (!found || cand < best) {
                        best = cand;
                        found = true;
                    }
                }
            }
        }
        m = next_prime(m + 1);                 /* try next m */
    }

    return found ? best : 0;
}
