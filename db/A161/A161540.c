/*
 * A161540  Primes which are the sum of a smaller n and its composite(n)
 *          subscript. A002808 + a(n) (= prime).
 * https://oeis.org/A161540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 830 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161540: primes p = k + composite(k) where composite(k) is the
 * k‑th composite number (starting with 4).  Only those sums that are prime
 * are stored; a(n) is the n‑th such prime, indexed by order of appearance.
 * The sequence is generated lazily: we scan composite numbers, compute
 * their sum with the corresponding index, test primality, and cache the
 * results.  Subsequent calls use the cache.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the n‑th term of A161540 (1‑based index).  */
seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* capacity of the cache array */

    /* state used to generate composite numbers and the corresponding sums */
    static seq_t m    = 1;  /* current composite index (k) */
    static seq_t comp = 4;  /* the k‑th composite number */

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)          /* allocation failed – return a safe value */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Ensure that `comp` is a composite number. */
        while (is_prime_big(comp))
            comp++;

        /* Candidate prime = composite index + composite number. */
        seq_t cand = m + comp;
        if (is_prime_big(cand))
            cache[cache_len++] = cand;

        /* Advance to the next composite number and index. */
        m++;
        comp++;
    }

    return cache[n - 1];
}
