/*
 * A182686  a(n) = the largest n-digit number with exactly 13 divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the largest n‑digit number of the form p^12 (p = prime).
 *            Returns 0 if no such number exists
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 961 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
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

/* Compute base^12 or return limit+1 if the multiplication would
   exceed limit (used in binary search to avoid overflow). */
static seq_t safe_pow12_le(seq_t base, seq_t limit)
{
    seq_t result = 1;
    for(int i = 0; i < 12; ++i)
    {
        /* If result*base would exceed limit, break */
        if (result > limit / base)
            return limit + 1;          /* sentinel > limit */
        result *= base;
    }
    return result;
}

/* Binary search for floor(12th root) of X; uses safe_pow12_le to avoid
   overflow for 64‑bit builds. */
static seq_t root12_floor(seq_t X)
{
    idx_t k        = num_digits(X) - 1;       /* floor(log10(X))   */
    idx_t low_exp  = k / 12;                 /* floor(k/12)       */
    idx_t high_exp = low_exp + 1;
    seq_t low  = pow10(low_exp);
    seq_t high = pow10(high_exp);
    seq_t best = 0;

    while (low <= high)
    {
        seq_t mid  = (low + high) / 2;
        seq_t mid12 = safe_pow12_le(mid, X);

        if (mid12 <= X)          /* mid^12 fits, try larger */
        {
            best = mid;
            low  = mid + 1;
        }
        else                     /* mid^12 too big, try smaller */
        {
            if (mid == 0) break; /* safety against underflow    */
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Small n have no solutions.  */
    if (n < 4) return 0;

    /* Lower bound on p: ceil(10^(n-1)^(1/12)) */
    seq_t ten_n_minus1 = pow10(n - 1);
    seq_t low = root12_floor(ten_n_minus1);
    if (safe_pow12_le(low, ten_n_minus1) < ten_n_minus1) low += 1;

    /* Upper bound on p: floor((10^n-1)^(1/12)) */
    seq_t ten_n = pow10(n);
    seq_t high = root12_floor(ten_n);
    if (ipow_i32(high, 12) >= ten_n) high -= 1;

    if (low > high) return 0;              /* no prime in range */

    /* Search downwards for the largest prime in [low, high] */
    seq_t p = high;
    while (true)
    {
        if (is_prime_big(p))
            return ipow_i32(p, 12);
        if (p == low) break;
        --p;
    }
    return 0; // no prime found
}
