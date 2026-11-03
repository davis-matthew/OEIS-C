/*
 * A163499  Take A163498(n) written in binary, insert a 0 before every 1. a(n)
 *          is the decimal equivalent of the result.
 * https://oeis.org/A163499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth number obtained by taking a prime p from sequence
 *            A163498 and inserting a 0 before every 1 in the binary
 *            representation of p. The result is returned as a seq_t
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 60 terms through n = 1975 (2026-11-03)
 * Helpers:   bit_length, is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bit_length.h"
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

/* Insert a 0 before every 1 in the binary representation of p.
   For example, if p = 5 (binary 101), the function returns
   1001 (decimal 9). */
static inline seq_t transform(seq_t p)
{
    seq_t res = 0;
    int bits = (int)bit_length(p);
    for (int i = bits - 1; i >= 0; --i) {
        res <<= 1;                            /* output 0 */
        if (((p >> i) & 1) != 0) {            /* original bit 1 -> output 1 */
            res <<= 1;
            res |= 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) abort(); /* OEIS offset starts at 1 */

    idx_t count = 0;
    idx_t candidate = 2;

    while (1) {
        if (is_prime(candidate)) {
            seq_t val = transform((seq_t)candidate);
            if (is_prime_big(val)) {
                ++count;
                if (count == n) return val;
            }
        }
        candidate += (candidate == 2 ? 1 : 2); /* iterate over odd numbers only */
    }
}
