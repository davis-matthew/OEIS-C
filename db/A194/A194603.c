/*
 * A194603  Smallest prime either of the form n*2^k - 1 or n*2^k + 1, k >= 0,
 *          or 0 if no such prime exists.
 * https://oeis.org/A194603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 240, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 889 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Smallest prime of the form n*2^k ± 1 (k ≥ 0), or 0 if none exists. */
/* The function uses a straightforward search over increasing k. */

#include <stdbool.h>
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

#define MAX_K 200  /* limit to avoid infinite loops if no prime is found */

seq_t a(idx_t n)
{
    /* Index starts at 1 */
    seq_t tmp = (seq_t)n;   /* n*2^k, starting with k=0 */

    for (idx_t k = 0;; ++k) {
        /* Process the current k */
        if (k > 0) {
            /* Multiply by 2, detect overflow for unsigned integer types */
            seq_t prev = tmp;
            tmp = tmp * (seq_t)2;
            if (tmp < prev)
                break;                    /* overflow → stop searching */
        }

        /* val1 = n*2^k - 1 (skip if <= 0) */
        if (tmp > (seq_t)1) {
            seq_t val1 = tmp - (seq_t)1;
            if (is_prime_big(val1))
                return val1;
        }

        /* val2 = n*2^k + 1 */
        seq_t val2 = tmp + (seq_t)1;
        if (is_prime_big(val2))
            return val2;

        /* Stop if we have exceeded our practical search limit */
        if (k >= MAX_K)
            break;
    }

    return (seq_t)0;  /* no prime found within the search limits */
}
