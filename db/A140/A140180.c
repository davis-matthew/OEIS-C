/*
 * A140180  A number n is included if the smallest prime that is congruent to
 *          1 (mod n) is <= the smallest prime that is congruent to -1 (mod
 *          n).
 * https://oeis.org/A140180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 567, u32 n <= 567, u64 n <= 567,
 *            u128 n <= 567, big any n
 * Validated: standard, pass, 79 terms through n = 523 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A140180: a(n) is the nth positive integer k such that
   the smallest prime ≡ 1 (mod k) is ≤ the smallest prime ≡ -1 (mod k).
   The function scans integers k=1,2,3,…, testing the property and
   returns the nth one.
*/
#include <stdbool.h>
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

static seq_t first_prime_residue(idx_t n, idx_t r)
{
    /* Special case when n==1: every prime is ≡ 0 (mod 1), so return 2 */
    if (n == 1) return (seq_t)2;

    seq_t cand = (seq_t)r;
    /* Ensure the candidate is at least 2 and congruent to r mod n */
    if (cand < (seq_t)2) cand += (seq_t)n;

    /* Step by n to stay in the same residue class */
    const seq_t step = (seq_t)n;
    while (1) {
        if (is_prime_big(cand)) return cand;
        cand += step;
    }
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 1;

    while (1) {
        bool ok;
        if (k == 1) {
            ok = true;                     /* 1 always satisfies the property */
        } else {
            seq_t p1    = first_prime_residue(k, 1);          /* 1 (mod k)  */
            seq_t p_minus = first_prime_residue(k, k - 1);    /* -1 (mod k) */
            ok = (p1 <= p_minus);
        }

        if (ok) {
            found++;
            if (found == n)
                return (seq_t)k;
        }
        k++;
    }
}
