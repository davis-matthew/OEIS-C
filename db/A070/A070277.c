/*
 * A070277  Smallest prime ending in n, or 0 if no such prime exists.
 * https://oeis.org/A070277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1028, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9497 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits_idx_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070277:
   a(n) = the smallest prime ending with the decimal representation of n,
   or 0 if no such prime exists.  The function searches over numbers of
   the form n + k·10^d, where d is the number of digits in n.  It
   immediately returns 0 for suffixes that cannot be prime
   (even last digits other than 2 and odd last digits 5).  For all other
   cases it performs a fast prime test on each candidate. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits_idx_to_idx.h"
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

/* Euclidean GCD for idx_t (assumed to be unsigned).  */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0U) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;          /* safety for index 0 */

    /* Fast elimination of impossible suffixes. */
    uint8_t last = (uint8_t)(n % 10U);
    if (last == 0U)
        return (seq_t)0;
    if (last == 2U)
        return (n == 2U) ? (seq_t)2 : (seq_t)0;
    if (last == 4U || last == 6U || last == 8U)
        return (seq_t)0;
    if (last == 5U)
        return (n == 5U) ? (seq_t)5 : (seq_t)0;

    /* Determine the modulus 10^d.  */
    idx_t d = num_digits_idx_to_idx(n);
    idx_t mod_idx = 1U;
    for (idx_t i = 0; i < d; ++i)
        mod_idx *= 10U;

    /* If gcd(n,10^d) != 1, no prime in this arithmetic progression. */
    if (gcd_idx(n, mod_idx) != 1U)
        return (seq_t)0;

    /* Convert modulus to seq_t for arithmetic on candidates. */
    seq_t mod_seq = (seq_t)mod_idx;
    seq_t candidate = (seq_t)n;

    /* Search for the smallest prime in the progression. */
    while (true) {
        if (candidate >= (seq_t)2 &&
            is_prime_big(candidate))
            return candidate;

        candidate += mod_seq;
    }
}
