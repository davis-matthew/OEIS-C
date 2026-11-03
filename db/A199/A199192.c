/*
 * A199192  Primes of the form 3^(2n-3)+3^(n-1)+1.
 * https://oeis.org/A199192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199192: primes of the form 3^(2k‑3)+3^(k‑1)+1.
   a(n) returns the n‑th prime of this form, with offset 1.  The
   function repeatedly generates the candidate value and tests it
   for primality. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow3.h"
#include "../utils/is_prime_u64.h"

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
    if (n == 0) return 0;           /* offset 1 – this case is not used */

    /* start with k = 2 (the first value that yields 7) */
    idx_t k = 2;
    seq_t pow_odd  = pow3(1);       /* 3^(k‑1)  for k=2 -> 3^1 */
    seq_t pow_even = pow3(1);       /* 3^(2k‑3) for k=2 -> 3^1 */
    idx_t count = 0;

    while (1)
    {
        /* candidate value = 3^(2k‑3) + 3^(k‑1) + 1 */
        seq_t val = pow_even + pow_odd + 1;

        if (is_prime_big(val))
        {
            ++count;
            if (count == n)
                return val;
        }

        /* move to the next k: multiply exponents by 3 and 9 */
        ++k;
        pow_odd  = mul_small(pow_odd, 3);   /* 3^(k‑1) = previous * 3 */
        pow_even = mul_small(pow_even, 9);  /* 3^(2k‑3) = previous * 9 */
    }
}
