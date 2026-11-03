/*
 * A182685  a(n) = the smallest n-digit number with exactly 13 divisors, a(n)
 *          = 0 if no such number exists.
 * https://oeis.org/A182685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A182685: a(n) = the smallest n‑digit number of the form p^12 (p prime).  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

/* 
   The function searches for the smallest prime p such that p^12 has exactly n digits.
   If no such prime exists (i.e., the interval of primes between 10^((n-1)/12) and 10^(n/12))
   is empty, the function returns 0.
*/
seq_t a(idx_t n) {
    /* n = 1..3 have no 13‑divisor number with that many digits */
    if (n < 4) return 0;

    /* Y = 10^(n-1)  →  lower bound for p^12 */
    seq_t Y = pow10(n - 1);

    /* Find ceil 12th root of Y via binary search:
       minimal integer p such that p^12 >= Y. */
    idx_t exp_half = (n - 1) / 12;

    seq_t low  = 1;
    seq_t high = pow10(exp_half + 1); /* high^12 >> Y guarantees high is an upper bound */

    seq_t exp12 = (seq_t)12;
    while (low < high) {
        seq_t mid = (low + high) / 2;

        seq_t mid_pow = ipow(mid, exp12);  /* mid^12 */

        if (mid_pow >= Y)
            high = mid;
        else
            low = mid + 1;
    }

    seq_t p = low;   /* candidate prime starts at ceil_root(Y,12) */

    /* Find the next prime ≥ p */
    while (!is_prime_big(p)) p++;

    /* Compute candidate value p^12 */
    seq_t val = ipow(p, exp12);

    /* Verify that val has exactly n digits.  If not, no such number. */
    return (num_digits(val) == n) ? val : 0;
}
