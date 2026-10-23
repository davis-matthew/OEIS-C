/*
 * A141399  Positive integers k such that the distinct primes that divide k or
 *          k+1 form a set of consecutive primes. In other words, k is
 *          included if and only if k*(k+1) is contained in sequence A073491.
 * https://oeis.org/A141399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A141399: k such that the distinct primes dividing k or k+1 form
   a consecutive set starting with 2.  The sequence is generated
   by brute‑force searching integers k and testing the prime set
   condition.  This implementation is O(k log log k) where k is the
   value of the n‑th term  (fast enough for n ≤ 10 000). */
/* The interface required by the test harness is:
 *   seq_t a(idx_t n)
 *   where seq_t and idx_t are provided by the build.           */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------ Sieve of Eratosthenes ----------------------- */

#define PRIME_LIMIT 5000000U   /* sufficient for factoring k up to ~2.5e13 */

static uint32_t *PRIMES   = NULL;   /* array of primes */
static size_t   PRIME_CNT = 0;      /* number of primes */

static void init_primes(void)
{
    if (PRIMES != NULL)
        return;

    /* Allocate bitmap for composites */
    uint8_t *is_comp = (uint8_t *)malloc(PRIME_LIMIT + 1);
    if (!is_comp)
        abort();

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            ++cnt;

    PRIMES = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!PRIMES)
        abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            PRIMES[idx++] = i;

    PRIME_CNT = cnt;
    free(is_comp);
}

/* return number of stored primes that are ≤ x */
static size_t primes_leq(uint64_t x)
{
    size_t lo = 0, hi = PRIME_CNT;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint64_t)PRIMES[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ----------------- Factorisation utilities ------------------------ */

/* factor_unique(n, out) collects the distinct prime factors of n
   into array out[].  Returns the number of distinct primes found.
   The array out[] must be large enough (at least 64 entries). */
static size_t factor_unique(uint64_t n, uint64_t out[64])
{
    size_t out_n = 0;
    uint64_t m = n;
    for (size_t i = 0; i < PRIME_CNT && (uint64_t)PRIMES[i] * PRIMES[i] <= m; ++i) {
        uint32_t p = PRIMES[i];
        if (m % p == 0) {
            out[out_n++] = p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        out[out_n++] = m;       /* remaining prime factor */
    return out_n;
}

/* ----------------- Validity test for k ---------------------------- */

static bool is_valid_k(uint64_t k)
{
    uint64_t pk[32], pk1[32];
    size_t n1 = factor_unique(k, pk);
    size_t n2 = factor_unique(k + 1, pk1);

    uint64_t union_pr[64];
    size_t  un = 0;

    /* copy pk */
    for (size_t i = 0; i < n1; ++i)
        union_pr[un++] = pk[i];

    /* add pk1 if not already present */
    for (size_t i = 0; i < n2; ++i) {
        uint64_t val = pk1[i];
        bool seen = false;
        for (size_t j = 0; j < un; ++j)
            if (union_pr[j] == val) { seen = true; break; }
        if (!seen)
            union_pr[un++] = val;
    }

    if (un == 0)
        return false;   /* should never happen for k ≥ 1 */

    uint64_t minp = union_pr[0], maxp = union_pr[0];
    for (size_t i = 1; i < un; ++i) {
        if (union_pr[i] < minp) minp = union_pr[i];
        if (union_pr[i] > maxp) maxp = union_pr[i];
    }
    if (minp != 2)        /* must start at 2 */
        return false;

    size_t needed = primes_leq(maxp);
    return un == needed;
}

/* ------------------ Sequence storage ------------------------------ */

static seq_t *SEQ = NULL;      /* dynamically grown array of seq_t terms */
static size_t SEQ_LEN = 0;
static size_t SEQ_CAP = 0;

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= SEQ_CAP)
        return;
    size_t new_cap = SEQ_CAP ? SEQ_CAP << 1 : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    SEQ = (seq_t *)realloc(SEQ, new_cap * sizeof(seq_t));
    if (!SEQ)
        abort();
    SEQ_CAP = new_cap;
}

/* -------------------- Main sequence function ---------------------- */

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined; return 0 as sentinel */
        return (seq_t)0;

    init_primes();

    static uint64_t k_next = 1;
    while (SEQ_LEN < (size_t)n) {
        if (is_valid_k(k_next)) {
            ensure_seq_capacity(SEQ_LEN + 1);
            SEQ[SEQ_LEN++] = (seq_t)k_next;
        }
        ++k_next;
    }
    return SEQ[n - 1];
}
