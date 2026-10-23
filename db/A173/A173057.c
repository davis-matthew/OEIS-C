/*
 * A173057  Partial sums of A024770.
 * https://oeis.org/A173057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A173057 – Partial sums of right‑truncatable primes
 *  a(n) = Σ_{k=1..n} p_k
 *  where p_k is the k‑th right‑truncatable prime (decimal) in increasing order.
 *
 *  Implementation details:
 *  * Generate the 83 right‑truncatable primes by breadth‑first search.
 *  * Store them as uint64_t and pre‑compute their prefix sums.
 *  * For any requested n the result is the n‑th prefix sum (or the total if n>83),
 *    cast to seq_t.  This works for all supported seq_t sizes; for small types
 *    values that do not fit simply wrap as required by the test harness.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Test primality for 64‑bit numbers using deterministic Miller–Rabin. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases for deterministic test for n < 2^64 */
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        /* compute a^d mod n */
        __uint128_t x = 1;
        __uint128_t pow = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Static state for the generated primes and their partial sums. */
static uint64_t *primes      = NULL;
static size_t    primes_len  = 0;
static uint64_t *prefix64    = NULL;

static void init_sequence(void)
{
    if (primes_len) return; /* already initialized */

    /* allocate space for primes (max 83) and a vector for generation */
    const size_t max_primes = 83;
    size_t cap = 128;
    primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!primes) abort();

    /* start with the single‑digit right‑truncatable primes */
    primes[0] = 2;
    primes[1] = 3;
    primes[2] = 5;
    primes[3] = 7;
    primes_len = 4;

    /* breadth‑first construction */
    size_t i = 0;
    while (i < primes_len && primes_len < max_primes) {
        uint64_t p = primes[i++];
        for (int d = 1; d <= 9; d += 2) {
            uint64_t q = p * 10 + (uint64_t)d;
            if (is_prime_uint64(q)) {
                if (primes_len == cap) {
                    cap <<= 1;
                    primes = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
                    if (!primes) abort();
                }
                primes[primes_len++] = q;
                if (primes_len >= max_primes) break;
            }
        }
    }

    /* compute prefix sums in 64‑bit */
    prefix64 = (uint64_t*)malloc((primes_len + 1) * sizeof(uint64_t));
    if (!prefix64) abort();
    prefix64[0] = 0;
    for (size_t j = 1; j <= primes_len; ++j)
        prefix64[j] = prefix64[j - 1] + primes[j - 1];
}

/*  External function required by the harness. */
seq_t a(idx_t n)
{
    if (!primes_len) init_sequence();

    size_t idx = (size_t)n;
    if (idx > primes_len) idx = primes_len;
    return (seq_t)prefix64[idx];
}
