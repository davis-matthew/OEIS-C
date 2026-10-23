/*
 * A115387  a(1) = 1. For n >= 2, a(n) = sum of the two (not necessarily
 *          distinct) earlier terms, a(j) and a(k), which maximizes
 *          d(a(j)+a(k)), where d(m) is the number of positive divisors of m.
 *          a(n) = the minimum (a(j)+a(k)) if more than one such sum has the
 *          maximum number of divisors.
 * https://oeis.org/A115387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n >= 2, a(n) is the sum of two (possibly
 *            identical) earlier terms a(j) + a(k) which maximizes
 *            d(a(j)+a(k)), where d(m) is the number of positive divisors of
 *            m. If several sums have the same maximal divisor count, the
 *            smallest sum is chosen
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations of helper functions from utils */
seq_t *alloc_seq_array(size_t n);  /* not used, but kept for compatibility */

/* ---------- Auxiliary 64‑bit integer helpers for divisor counting ---------- */

/* Small prime list for trial division */
static const uint32_t small_primes[] = {
    2u,3u,5u,7u,11u,13u,17u,19u,23u,29u,31u,37u,41u,43u,47u,
    53u,59u,61u,67u,71u,73u,79u,83u,89u,97u,101u,103u,107u,109u,113u,
    127u,131u,137u,139u,149u,151u,157u,163u,167u,173u,179u,181u,191u,193u,197u,199u
};
static const size_t NUM_SMALL_PRIMES = sizeof(small_primes)/sizeof(small_primes[0]);

/* Greatest common divisor for 64‑bit integers */
static uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Modular multiplication using __int128 */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* Modular exponentiation */
static uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* Deterministic Miller‑Rabin test for 64‑bit integers */
static bool is_prime_64(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < NUM_SMALL_PRIMES && (uint64_t)small_primes[i] * small_primes[i] <= n; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Deterministic bases for 64-bit integers */
    const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* Pollard‑Rho factorisation for composite 64‑bit integers */
static uint64_t pollard_rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd64(diff, n);
        }
        if (d != n) return d;
        ++c;
    }
}

/* Add prime factor to list, merging duplicates */
static void add_factor(uint64_t p, int *len, uint64_t *factors, int *exponents) {
    for (int i = 0; i < *len; ++i) {
        if (factors[i] == p) {
            ++exponents[i];
            return;
        }
    }
    factors[(*len)++] = p;
    exponents[(*len)-1] = 1;
}

/* Recursive factorisation collecting prime factors */
static void factor64(uint64_t n, int *len, uint64_t *factors, int *exponents) {
    if (n == 1) return;
    if (is_prime_64(n)) { add_factor(n, len, factors, exponents); return; }
    uint64_t d = pollard_rho(n);
    factor64(d, len, factors, exponents);
    factor64(n / d, len, factors, exponents);
}

/* Divisor count for a 64‑bit integer (used for seq_t values that fit in 64 bits) */
static int divisor_count_u64_wrapper(uint64_t n) {
    if (n == 0) return 0;
    int len = 0;
    uint64_t factors[64];
    int exponents[64];
    factor64(n, &len, factors, exponents);
    int result = 1;
    for (int i = 0; i < len; ++i) {
        result *= (exponents[i] + 1);
    }
    return result;
}

/* Generic divisor count for seq_t, assumes the value fits into 64 bits */
static int divisor_count_generic(seq_t value) {
    return divisor_count_u64_wrapper((uint64_t)value);
}

/* ---------- Sequence generation ---------- */

/* Static storage for the generated sequence */
static seq_t *seq_storage = NULL;
static size_t seq_capacity = 0;
static idx_t seq_computed = 0;

/* Current best sum and its divisor count */
static seq_t current_best_sum;
static int current_max_div;

/* Ensure the storage has enough capacity for new terms */
static void ensure_capacity(size_t needed) {
    if (seq_capacity < needed) {
        size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_storage = (seq_t *)realloc(seq_storage, new_cap * sizeof(seq_t));
        seq_capacity = new_cap;
    }
}

/* Compute the sequence up to index n (inclusive) */
static void compute_upto(idx_t n) {
    if (n <= seq_computed) return;

    /* Initialise the first term if not yet done */
    if (seq_computed == 0) {
        ensure_capacity(1);
        seq_storage[0] = (seq_t)1;
        seq_computed = 1;
        current_max_div = 0;
        current_best_sum = (seq_t)0;
    }

    /* Generate terms one by one */
    for (idx_t idx = seq_computed + 1; idx <= n; ++idx) {
        /* new term will be the best sum among all pairs of indices < idx */
        /* Update current_best_sum and current_max_div by considering sums with the newest term */
        seq_t last_val = seq_storage[idx - 2];   /* a(idx-1) */
        for (idx_t j = 1; j <= idx - 1; ++j) {
            seq_t sum = seq_storage[j - 1] + last_val;
            int divcnt = divisor_count_generic(sum);
            if (divcnt > current_max_div || (divcnt == current_max_div && sum < current_best_sum)) {
                current_max_div = divcnt;
                current_best_sum = sum;
            }
        }
        /* Store the new term */
        ensure_capacity(idx);
        seq_storage[idx - 1] = current_best_sum;
        ++seq_computed;
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n) {
    compute_upto(n);
    return seq_storage[n - 1];
}
