/*
 * A121573  Prime-gap race; difference of the cumulative sums of gaps above
 *          and below prime(2n).
 * https://oeis.org/A121573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, sign, look
 * Formula:   a(n) = Sum_{k=1..n} (prime(2k+1)-prime(2k)) - Sum_{k=1..n}
 *            (prime(2k)-prime(2k-1))
 * Range:     i8 n <= 390, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- prime storage (0‑based) ---------- */
static uint32_t *prime_list = NULL;   /* prime_list[i] == prime(i+1) */
static size_t    prime_len  = 0;      /* number of primes stored   */
static size_t    prime_cap  = 0;      /* capacity of prime_list    */

/* Ensure at least 'need' primes are stored (prime_list[0..need-1]) */
static void
ensure_primes(size_t need)
{
    if (prime_len >= need)
        return;

    uint64_t cand = prime_len ? (uint64_t)prime_list[prime_len - 1] + 1ULL : 2ULL;

    while (prime_len < need) {
        /* find next prime starting from cand */
        while (true) {
            bool ok = true;
            for (size_t i = 0; i < prime_len &&
                 (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= cand; ++i) {
                if (cand % prime_list[i] == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                break;
            ++cand;
        }

        /* store the found prime */
        if (prime_len == prime_cap) {
            prime_cap = prime_cap ? prime_cap * 2 : 16;
            uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            if (!tmp)
                abort();
            prime_list = tmp;
        }
        prime_list[prime_len++] = (uint32_t)cand;
        ++cand;  /* next candidate will be larger than the prime we just added */
    }
}

/* ---------- a(n) cache (1‑based) ---------- */
static seq_t *a_cache = NULL;   /* a_cache[0] used as 0 */
static size_t a_len  = 0;        /* number of stored values (a_cache[0] to a_cache[a_len-1]) */
static size_t a_cap  = 0;        /* capacity of a_cache */

/* Ensure a_cache has room for indices up to 'max_index' (inclusive). */
static void
ensure_cache(size_t max_index)
{
    if (a_cap <= max_index) {
        a_cap = max_index + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        a_cache = tmp;
    }
}

/* ---------- sequence a(n) ---------- */
seq_t
a(idx_t n)
{
    if (n == 0)            /* safety, OEIS offset starts at 1 */
        return 0;

    /* We need primes up to prime(2n+1), i.e., first 2n+1 primes. */
    size_t need_primes = 2 * (size_t)n + 1;
    ensure_primes(need_primes);

    /* Grow cache if necessary and fill missing values. */
    if (a_len <= (size_t)n) {
        ensure_cache((size_t)n);
        if (a_len == 0) {          /* initialise a_cache[0] = 0 */
            a_cache[0] = 0;
            a_len     = 1;
        }
        for (size_t k = a_len; k <= (size_t)n; ++k) {
            /* zero‑based indices of the relevant primes */
            size_t idx_up   = 2 * k;       /* prime(2k+1) */
            size_t idx_mid  = idx_up - 1;  /* prime(2k)   */
            size_t idx_down = idx_mid - 1; /* prime(2k-1) */

            seq_t up   = (seq_t)prime_list[idx_up]   - (seq_t)prime_list[idx_mid];
            seq_t down = (seq_t)prime_list[idx_mid]  - (seq_t)prime_list[idx_down];

            a_cache[k] = a_cache[k - 1] + up - down;
        }
        a_len = (size_t)n + 1;
    }

    return a_cache[(size_t)n];
}
