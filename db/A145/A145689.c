/*
 * A145689  Primitive prime factors of the sequence 4k^2 + 1 in the order that
 *          they are found.
 * https://oeis.org/A145689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth primitive prime factor of 4k^2+1, found by
 *            scanning k = 1,2,3,... and recording each new prime that divides
 *            4k^2+1
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "../utils/gcd64.h"

/* --- 64‑bit modular arithmetic ------------------------------------ */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1;
    while (e) {
        if (e & 1)
            res = mulmod64(res, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* --- 64‑bit primality test (deterministic Miller–Rabin) ------------ */
static bool is_prime64(uint64_t n) {
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* --- Pollard–Rho factorisation ------------------------------------- */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0)
        return 2;
    static uint64_t seed = 2;
    uint64_t c = seed++;
    while (1) {
        uint64_t x = rand() % (n - 2) + 2;
        uint64_t y = x;
        uint64_t d = 1;
        while (d == 1) {
            x = (mulmod64(x, x, n) + c) % n;
            y = (mulmod64(y, y, n) + c) % n;
            y = (mulmod64(y, y, n) + c) % n;
            d = gcd64(x > y ? x - y : y - x, n);
        }
        if (d != n)
            return d;
        c++;
    }
}

static void factor_uint64(uint64_t n, uint64_t *buf, size_t *cnt) {
    if (n == 1)
        return;
    if (is_prime64(n)) {
        buf[(*cnt)++] = n;
        return;
    }
    uint64_t d = pollard(n);
    factor_uint64(d, buf, cnt);
    factor_uint64(n / d, buf, cnt);
}

/* --- Dynamic array helpers ------------------------------------------ */
static void grow_seq_arr(seq_t * *arr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : *cap << 1;
        if (new_cap < need)
            new_cap = need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* --- The sequence ---------------------------------------------------- */
seq_t a(idx_t n) {
    /* static state */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t last_k = 0;
    static bool init = false;

    if (!init) {
        init = true;
        srand((unsigned)time(NULL));
    }

    while (primes_len < (size_t)n) {
        ++last_k;
        uint64_t k = (uint64_t)last_k;
        uint64_t m = 4ULL * k * k + 1ULL;

        uint64_t buf[64];
        size_t bufcnt = 0;
        factor_uint64(m, buf, &bufcnt);

        /* insert distinct primes that are new */
        for (size_t i = 0; i < bufcnt; ++i) {
            uint64_t p = buf[i];
            bool seen = false;
            for (size_t j = 0; j < primes_len; ++j) {
                if (primes[j] == (seq_t)p) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                grow_seq_arr(&primes, &primes_cap, primes_len + 1);
                primes[primes_len++] = (seq_t)p;
                if (primes_len >= (size_t)n)
                    break;
            }
        }
    }
    return primes[(size_t)n - 1];
}
