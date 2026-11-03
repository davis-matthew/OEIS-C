/*
 * A158085  Starting at a(1)=2, a(n) is the smallest prime larger than a(n-1)
 *          such that the sum of odd digits of a(n) is not smaller than the
 *          sum of odd digits of a(n-1).
 * https://oeis.org/A158085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = smallest prime > a(n‑1) with sum of odd digits >= sum of
 *            odd digits of a(n‑1); a(1)=2
 * Range:     u8 n <= 13, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
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

/* Sum of odd decimal digits of x */
static inline seq_t sum_odd_digits(seq_t x) {
    seq_t sum = 0;
    while (x > 0) {
        seq_t d = x % 10;
        if (d & 1) sum += d;
        x /= 10;
    }
    return sum;
}

/* OEIS sequence A158085 */
seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    if (n == 0) return 0; /* not used, but guard */

    /* Build sequence up to requested index */
    while (seq_len < n) {
        /* Ensure capacity */
        if (seq_len >= seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 8;
            seq = mem_resize(seq_t, seq, seq_cap);
        }

        if (seq_len == 0) {                 /* first term */
            seq[0] = (seq_t)2;
            seq_len = 1;
            continue;
        }

        seq_t prev = seq[seq_len - 1];
        seq_t prevSum = sum_odd_digits(prev);

        seq_t cand = prev + 1;
        seq_t candSum;
        for (;;) {
            if (!is_prime_big(cand)) { cand++; continue; }

            candSum = sum_odd_digits(cand);
            if (candSum >= prevSum) {
                seq[seq_len] = cand;
                seq_len++;
                break;
            }
            cand++;
        }
    }

    return seq[n - 1];
}
