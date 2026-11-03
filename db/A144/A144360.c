/*
 * A144360  Primes of the form 8^k + 7. Also, primes of the form 64^m + 7.
 * https://oeis.org/A144360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form 64^m + 7 (m ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    static seq_t *primes   = NULL;
    static size_t  primes_cnt = 0;
    static size_t  primes_cap = 0;
    static seq_t  current_power = 0;
    static bool   initialized  = false;

    if (!initialized) {
        /* initial buffer for primes */
        primes_cap   = 16;
        primes       = mem_new(seq_t, primes_cap);
        primes_cnt   = 0;
        /* start with m = 1 : 64^1 */
        current_power = mul_small((seq_t)1, 64);
        initialized  = true;
    }

    /* generate primes until we have at least n of them */
    while (primes_cnt < n) {
        /* candidate = 64^m + 7 */
        seq_t candidate = current_power;
        candidate += (seq_t)7;

        if (is_prime_big(candidate)) {
            /* store the prime */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *new_arr = mem_resize(seq_t, primes, new_cap);
                if (!new_arr) abort();
                primes     = new_arr;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }

        /* advance to next m : multiply by 64 to get 64^(m+1) */
        current_power = mul_small(current_power, 64);
    }

    return primes[n - 1];
}
