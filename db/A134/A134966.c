/*
 * A134966  Primes that use all of the prime digits 2,3,5,7 exactly once.
 * https://oeis.org/A134966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime that uses each of the digits 2,3,5,7 exactly once */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* generate all primes containing the digits 2,3,5,7 exactly once */
static void generate_primes(seq_t *out, size_t *cnt) {
    const int d[4] = {2, 3, 5, 7};
    seq_t temp[24];
    size_t sz = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) if (j != i) {
            for (int k = 0; k < 4; ++k) if (k != i && k != j) {
                for (int l = 0; l < 4; ++l) if (l != i && l != j && l != k) {
                    seq_t val = d[i];
                    val = val * 10 + d[j];
                    val = val * 10 + d[k];
                    val = val * 10 + d[l];
                    if (is_prime_big(val)) {
                        temp[sz++] = val;
                    }
                }
            }
        }
    }

    /* simple insertion sort for at most 8 elements */
    for (size_t m = 1; m < sz; ++m) {
        seq_t key = temp[m];
        size_t n = m;
        while (n > 0 && temp[n - 1] > key) {
            temp[n] = temp[n - 1];
            --n;
        }
        temp[n] = key;
    }

    for (size_t i = 0; i < sz; ++i) out[i] = temp[i];
    *cnt = sz;
}

seq_t a(idx_t n) {
    static seq_t primes[24];
    static size_t prime_cnt = 0;
    if (prime_cnt == 0) {
        generate_primes(primes, &prime_cnt);
    }
    if (n == 0 || n > prime_cnt) return 0;
    return primes[n - 1];
}
