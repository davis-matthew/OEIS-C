/*
 * A233542  Number of ways to write n = k^2 + m with k > 0 and m > 0 such that
 *          phi(k^2)*phi(m) - 1 is prime, where phi(.) is Euler's totient
 *          function (A000010).
 * https://oeis.org/A233542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9841 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the number of representations n = k^2 + m (k>0, m>0)
   such that phi(k^2)*phi(m) - 1 is a prime.
   a(n) = #{k, m ∈ ℕ : k>0, m>0, k^2 + m = n,
               is_prime(phi(k^2)*phi(m) - 1)} */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt.h"
#include "../utils/totient_seq.h"
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
    seq_t cnt = 0;
    if (n <= 5) return 0;          /* handle small n explicitly */
    idx_t maxk = isqrt(n - 1);  /* largest k with k^2 < n */
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t sq = k * k;
        idx_t m   = n - sq;
        seq_t phi_k2 = totient_seq((seq_t)sq);
        seq_t phi_m   = totient_seq((seq_t)m);
        seq_t val = phi_k2 * phi_m - 1;
        if (is_prime_big(val))
            ++cnt;
    }
    return cnt;
}
