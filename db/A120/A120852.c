/*
 * A120852  Triangle read by rows, where t(n,1) = 1, t(n,m) = t(n,m-1) +
 *          (largest noncomposite {1 or prime} in row {n-1}).
 * https://oeis.org/A120852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the nth element of the triangle read by rows, where
 *            t(1,1)=1, and for n>1, t(n,1)=1, t(n,m)=t(n,m-1)+L_{n-1}, with
 *            L_{n-1} = the largest prime or 1 in row (n-1)
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------- Standard includes -------------- */
#include <stdbool.h>
#include <stddef.h>

/* -------------- Utility includes -------------- */
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

/* -------------- Sequence function -------------- */
seq_t a(idx_t n) {
    seq_t diff = 1;      /* L_{previous row}  (difference for current row) */
    idx_t  row_len = 1;  /* length of current row */
    idx_t  idx = 1;      /* current index in the flattened triangle */

    while (true) {
        /* Generate the current row */
        seq_t val = 1;
        seq_t largest_prime = 1;   /* 1 is considered the default non‑composite */

        for (idx_t m = 1; m <= row_len; ++m) {
            /* If we have reached the requested index, return the value */
            if (idx == n) {
                return val;
            }

            /* Update largest prime seen in this row */
            if (is_prime_big(val)) {
                largest_prime = val;   /* since values increase, this will be the largest prime */
            }

            /* Prepare value for next element in the row */
            val += diff;
            ++idx;
        }

        /* Prepare for next row */
        diff = largest_prime;   /* largest prime in this row (or 1 if none) */
        ++row_len;              /* next row will be one element longer */
    }
}
