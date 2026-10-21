/*
 * A069521  Smallest multiple of n with digit sum = 2, or 0 if no such number
 *          exists, e.g., a(3k)=0.
 * https://oeis.org/A069521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   pow10_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069521: Smallest multiple of n with digit sum = 2 */

/* 
 * Definition: 
 *   If n = 2^a * 5^b then  a(n) = 2 * 10^max(a-1, b)
 *   If n = 2^a * 5^b * c where c is in A043292 (numbers coprime to 10 for which 10 has an exponent e with 10^e ≡ -1 (mod c))
 *      then a(n) = 10^max(a,b) * (1 + 10^e)
 *   Otherwise a(n) = 0
 *
 *   Here e is the smallest positive integer with 10^e ≡ -1 (mod c), i.e. e = ord(10, c)/2
 *   where ord(10, c) is the multiplicative order of 10 modulo c.
 *
 *   The function below implements the above definition.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10_exp.h"

/* modular exponentiation: (base^exp) mod mod, all uint64_t */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* compute Euler's totient and the multiplicative order of 10 modulo n.
 *  phi_out  : phi(n)
 *  ord_out  : ord(10, n)
 *  Returns true on success, false if n==0 (should not happen). */
static bool phi_and_order_uint64(uint64_t n, uint64_t *phi_out, uint64_t *ord_out)
{
    if (n == 0) return false;
    uint64_t phi = n;
    uint64_t temp = n;
    /* factor n to compute phi */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            phi -= phi / p;
        }
    }
    if (temp > 1) phi -= phi / temp;

    uint64_t ord = phi;

    /* factor ord to reduce it to the order of 10 modulo n */
    temp = ord;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            while (ord % p == 0 &&
                   pow_mod_uint64(10, ord / p, n) == 1) {
                ord /= p;
            }
        }
    }
    if (temp > 1) {
        while (ord % temp == 0 &&
               pow_mod_uint64(10, ord / temp, n) == 1) {
            ord /= temp;
        }
    }

    *phi_out = phi;
    *ord_out = ord;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, guard anyway */

    uint64_t tn = (uint64_t)n;      /* copy into a native type for manipulation */

    /* count powers of 2 and 5 */
    unsigned a2 = 0;
    while ((tn & 1ULL) == 0) { a2++; tn >>= 1; }

    unsigned a5 = 0;
    while (tn % 5ULL == 0) { a5++; tn /= 5; }

    uint64_t c = tn;                /* remaining part, gcd(c,10)==1 */

    /* Case 1: n = 2^a * 5^b */
    if (c == 1) {
        int shiftExp;               /* exponent of 10 */
        if (a2 > 0) {               /* n has a factor 2 */
            shiftExp = (int)a2 - 1;
            if ((int)a5 > shiftExp) shiftExp = (int)a5;
        } else {                    /* n has no factor 2, only 5^b */
            shiftExp = (int)a5;
        }
        seq_t ans = pow10_exp((idx_t)shiftExp);
        ans *= (seq_t)2;
        return ans;
    }

    /* Remaining case: n = 2^a * 5^b * c, c > 1 and coprime to 10 */
    /* Compute phi and order of 10 modulo c */
    uint64_t phi, ord;
    phi_and_order_uint64(c, &phi, &ord);

    /* If the order is odd, no solution */
    if (ord & 1ULL) return (seq_t)0;

    /* Verify that 10^(ord/2) ≡ -1 (mod c) */
    if (pow_mod_uint64(10, ord / 2ULL, c) != c - 1) return (seq_t)0;

    /* e = ord/2 */
    uint64_t e = ord / 2ULL;

    /* shift exponent = max(a2, a5) */
    int shiftExp = (a2 > a5) ? (int)a2 : (int)a5;

    seq_t part1 = pow10_exp((idx_t)shiftExp);          /* 10^max(a,b) */
    seq_t part2 = pow10_exp(e);                        /* 10^e  */
    part2 += (seq_t)1;                                 /* 1 + 10^e */

    return part1 * part2;
}
