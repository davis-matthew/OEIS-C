/*
 * A184859  Primes of the form floor(kr+h), where r=(1+sqrt(5))/2 and h=1/2.
 * https://oeis.org/A184859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime among U(k) = floor(k*(1+√5)/2 + 1/2) U(k)
 *            can be computed without floating point: U(k) = floor((k +
 *            floor_sqrt(5*k*k) + 1) / 2)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* compute U(k) = floor(k*φ + 1/2) where φ = (1+√5)/2
   using integer arithmetic: U(k) = floor((k + floor_sqrt(5*k*k) + 1) / 2) */
static seq_t U_value(idx_t k)
{
    /* 5*k*k may overflow a 64‑bit type, so do it in 128‑bit first. */
    unsigned __int128 prod = (unsigned __int128)5 * k * k;
    seq_t prod_s = (seq_t)prod;                     /* safe for all needed k */
    seq_t sqrt_v = floor_sqrt(prod_s);              /* floor(sqrt(5*k*k))   */
    return (k + sqrt_v + 1) / 2;                    /* integer division     */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* undefined below offset */

    idx_t found = 0;
    idx_t k = 1;
    seq_t result = 0;

    while (found < n) {
        seq_t u = U_value(k);
        if (is_prime_big(u)) {
            ++found;
            if (found == n) {
                result = u;
                break;
            }
        }
        ++k;
    }
    return result;
}
