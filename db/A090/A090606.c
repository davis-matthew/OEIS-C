/*
 * A090606  A089144 indexed by A000040.
 * https://oeis.org/A090606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090606: a(n) = index k of the nth prime p_k such that 6·p_k-5 and 6·p_k-1 are prime */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- primality test for 64‑bit integers ---------- */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t modpow(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = mulmod(res, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for all 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- sequence machinery ---------- */
static idx_t *qual_idx = NULL;    /* indices k of qualifying primes   */
static size_t qual_cap = 0;
static size_t qual_cnt = 0;
static idx_t prime_cnt = 0;       /* current prime index (1‑based)   */
static idx_t candidate = 2;       /* next candidate to test for prime */

static void ensure_cap(size_t needed) {
    if (needed <= qual_cap) return;
    size_t new_cap = qual_cap == 0 ? 16 : qual_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    qual_idx = (idx_t *)realloc(qual_idx, new_cap * sizeof(idx_t));
    qual_cap = new_cap;
}

/* ---------- public function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – undefined for n<1 */

    /* generate terms until we have at least n qualifying indices */
    while (qual_cnt < n) {
        /* find next prime */
        while (!is_prime_u64((uint64_t)candidate)) {
            candidate = (candidate == 2 ? 3 : candidate + 2);
        }
        ++prime_cnt;
        idx_t k = prime_cnt;

        /* test the property */
        seq_t m1 = (seq_t)6 * candidate - 5;
        seq_t m2 = (seq_t)6 * candidate - 1;
        if (is_prime_u64((uint64_t)m1) && is_prime_u64((uint64_t)m2)) {
            ensure_cap(qual_cnt + 1);
            qual_idx[qual_cnt++] = k;
        }

        /* advance to next candidate */
        candidate = (candidate == 2 ? 3 : candidate + 2);
    }

    return (seq_t)qual_idx[n - 1];
}
