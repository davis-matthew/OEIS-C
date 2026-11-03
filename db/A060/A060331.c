/*
 * A060331  Primes the sum of six consecutive composite numbers.
 * https://oeis.org/A060331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 460, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 991 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth prime that can be written as the sum of six consecutive
   composite numbers.  The algorithm walks through the integers, keeps
   a sliding window of the last six composites, and tests whether the
   window sum is prime.  The first prime found corresponds to a(1). */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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
    /* Offset 1: return value of a(1) is the first prime found. */
    if (n == 0) return 0;               /* behaviour below offset is irrelevant */

    seq_t window[6] = {0};              /* last six composites (as seq_t)   */
    seq_t sum = 0;                      /* sum of the current window        */
    idx_t count = 0;                    /* number of composites seen */
    idx_t found = 0;                    /* how many prime sums found */

    idx_t m = 4;                        /* start checking from 4            */

    while (1) {
        /* Skip primes; composite numbers are 4 or more and not prime. */
        if (m >= 4 && !is_prime(m)) {
            seq_t val = (seq_t)m;

            if (count < 6) {            /* filling the initial window   */
                window[count++] = val;
                sum += val;

                if (count == 6 && is_prime_big(sum)) {
                    ++found;
                    if (found == n) return sum;
                }
            } else {
                /* Slide window: drop the oldest composite. */
                sum -= window[0];
                for (idx_t i = 0; i < 5; ++i) window[i] = window[i + 1];
                window[5] = val;
                sum += val;

                if (is_prime_big(sum)) {
                    ++found;
                    if (found == n) return sum;
                }
            }
        }
        ++m;
    }
}
