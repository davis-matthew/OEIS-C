/*
 * A014847  Numbers k such that k-th Catalan number C(2k,k)/(k+1) is divisible
 *          by k.
 * https://oeis.org/A014847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7459, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014847: 
 * a(n) = the nth positive integer k such that the k‑th Catalan number
 *       C(2k,k)/(k+1) is divisible by k.  Equivalently binom(2k,k) ≡ 0 (mod k). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helper functions ---------- */

static inline idx_t legendre(idx_t n, idx_t p) {
    idx_t res = 0;
    while (n) {
        n /= p;
        res += n;
    }
    return res;
}

/* Tests whether k satisfies binom(2k,k) divisible by k. */
static bool good_k(idx_t k, const uint64_t *spf) {
    idx_t tmp = k;
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        if (p == 0) p = tmp;                 /* safety for 1 or primes */
        idx_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        /* exponent of p in binom(2k,k) */
        idx_t exp = legendre(2 * k, p) - 2 * legendre(k, p);
        if (exp < e) return false;
    }
    return true;
}

/* ---------- global cache & environment ---------- */

#define SPF_LIMIT 200000          /* adjust if higher values of n are required */

static seq_t *seq_cache = NULL;   /* cached sequence values */
static size_t   cache_len  = 0;   /* number of terms cached */
static size_t   cache_cap  = 0;   /* allocated capacity of seq_cache */
static idx_t    next_k      = 1;   /* smallest k to test next */

static uint64_t *global_spf = NULL;   /* smallest prime factor array */

/* initializes the smallest‑prime‑factor sieve */
static void init_spf(void) {
    if (global_spf)
        return;
    global_spf = (uint64_t*)calloc(SPF_LIMIT + 1, sizeof(uint64_t));
    for (idx_t i = 2; i <= SPF_LIMIT; ++i) {
        if (global_spf[i] == 0) {
            global_spf[i] = i;
            if (i * (idx_t)i <= SPF_LIMIT) {
                for (idx_t j = i * i; j <= SPF_LIMIT; j += i)
                    if (global_spf[j] == 0)
                        global_spf[j] = i;
            }
        }
    }
}

/* ensures seq_cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need) {
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_ptr;
    cache_cap = new_cap;
}

/* ---------- main function ---------- */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                  /* sequence is 1‑based */

    init_spf();

    /* generate terms until we have n of them */
    while (cache_len < n) {
        /* test next_k for the property */
        if (good_k(next_k, global_spf)) {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq_cache[n - 1];
}
