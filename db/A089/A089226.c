/*
 * A089226  Smallest composite number with exactly n 1's in binary
 *          representation.
 * https://oeis.org/A089226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest composite number with exactly n 1's in binary
 *            representation. The function iterates over the integers with
 *            popcount n in increasing order using the classic algorithm to
 *            obtain the next integer with the same number of 1‑bits. It stops
 *            when a composite value is found
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 3254 (2026-11-03)
 * Helpers:   is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
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
    /* First candidate: the smallest number with n ones is 2^n−1. */
    seq_t val = pow2(n);
    val -= (seq_t)1;

    /* Search until a composite is found. */
    while (!((val > (seq_t)1) && !is_prime_big(val))) {
        /* next = next integer with the same popcount as val.
           Algorithm from
              Gosper's hack: lowbit = val & (~val + 1)
              next = val + lowbit
              right = ((val ^ next) >> 2) / lowbit
              val = next | right   */
        seq_t lowbit = val & (~val + (seq_t)1);  /* lowest set bit */
        seq_t next_val = val + lowbit;
        seq_t right = (val ^ next_val) >> 2;
        right /= lowbit;
        val = next_val | right;
    }

    return val;
}
