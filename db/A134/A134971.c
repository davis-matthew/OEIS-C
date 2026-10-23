/*
 * A134971  Canyon primes.
 * https://oeis.org/A134971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the nth prime canyon number (A134971)
 * Range:     u8 none, u16 none, u32 n <= 921, u64 n <= 9237, u128 n <= 9237,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* standard utilities */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* ---------- Miller–Rabin primality test for 64‑bit integers ---------- */
static inline uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t result = 1;
    a %= mod;
    while (e) {
        if (e & 1ull) result = mul_mod64(result, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return result;
}

/* deterministic bases for testing < 2^64 */
static const uint64_t mr_bases[] = {
    2ull, 325ull, 9375ull, 28178ull,
    450775ull, 9780504ull, 1795265022ull
};

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* small prime trial */
    static const uint64_t small_primes[] = {2ull,3ull,5ull,7ull,11ull,13ull,17ull,19ull,23ull,29ull,31ull,37ull};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ull) == 0ull) {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof(mr_bases)/sizeof(mr_bases[0]); ++i) {
        uint64_t a = mr_bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- end Miller–Rabin ---------- */

/* ---------- comparison for qsort ---------- */
static int cmp_seq_t(const void *p1, const void *p2) {
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b);
}

/* ---------- Canyon prime sequence ---------- */
seq_t a(idx_t n) {
    /* static cache: built once, reused on subsequent calls */
    static seq_t *canyon_primes = NULL;
    static idx_t  count_primes  = 0;
    static bool   initialized   = false;

    if (!initialized) {
        /* total number of canyon numbers = 116 505 (sum_{d=1}^9 (4^d-1)/3) */
        const idx_t total_cav = 116505;
        seq_t *cav = (seq_t *)malloc(sizeof(seq_t) * (size_t)total_cav);
        if (!cav) return 0; /* allocation failure */

        idx_t idx = 0;
        for (int d = 1; d <= 9; ++d) {          /* first digit */
            for (int m = 0; m < d; ++m) {        /* minimal digit */
                int range = d - m - 1;           /* digits between d and m, exclusive */
                int max_mask = 1 << range;       /* subsets of those digits */

                for (int maskD = 0; maskD < max_mask; ++maskD) {
                    for (int maskI = 0; maskI < max_mask; ++maskI) {
                        seq_t val = (seq_t)d;       /* first digit */

                        /* decreasing part (descending order) */
                        for (int i = range - 1; i >= 0; --i) {
                            if (maskD & (1 << i)) {
                                int dig = m + 1 + i;
                                val = val * 10 + dig;
                            }
                        }

                        /* minimal digit */
                        val = val * 10 + m;

                        /* increasing part (ascending order) */
                        for (int i = 0; i < range; ++i) {
                            if (maskI & (1 << i)) {
                                int dig = m + 1 + i;
                                val = val * 10 + dig;
                            }
                        }

                        /* last digit (same as first) */
                        val = val * 10 + d;

                        cav[idx++] = val;
                    }
                }
            }
        }
        /* The loop should have filled exactly total_cav entries */
        /* sort canyon numbers */
        qsort(cav, (size_t)total_cav, sizeof(seq_t), cmp_seq_t);

        /* count primes */
        count_primes = 0;
        for (idx_t i = 0; i < total_cav; ++i) {
            if (is_prime_u64((uint64_t)cav[i])) ++count_primes;
        }

        /* allocate array for primes */
        canyon_primes = (seq_t *)malloc(sizeof(seq_t) * (size_t)count_primes);
        if (!canyon_primes) {
            free(cav);
            return 0; /* allocation failure */
        }

        idx_t pidx = 0;
        for (idx_t i = 0; i < total_cav; ++i) {
            if (is_prime_u64((uint64_t)cav[i])) {
                canyon_primes[pidx++] = cav[i];
            }
        }

        free(cav);
        initialized = true;
    }

    if (n == 0 || n > count_primes) return 0;
    return canyon_primes[n - 1];
}
