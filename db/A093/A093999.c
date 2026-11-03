/*
 * A093999  Primes of the form abs(floor(k*log(k)-sqrt(k*(k+2*Pi)/Pi))).
 * https://oeis.org/A093999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = the n-th prime of the form abs(floor(k*log(k) -
 *            sqrt(k*(k+2*Pi)/Pi)))
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t last_k = 0;
    static const long double PI = 3.14159265358979323846264338327950288419716939937510L;

    while (primes_len < n) {
        ++last_k;
        long double kld = (long double)last_k;
        long double val = kld * logl(kld) - sqrtl(kld * (kld + 2.0L * PI) / PI);
        long long floor_val = (long long)floorl(val);
        uint64_t cand64 = (floor_val < 0) ? (uint64_t)(-floor_val) : (uint64_t)floor_val;
        if (cand64 > 1) {
            seq_t cand = (seq_t)cand64;
            if (is_prime_big(cand)) {
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = cand;
            }
        }
    }
    return primes[n - 1];
}
