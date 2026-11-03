/*
 * A168027  Noncomposite numbers in the southern ray of the Ulam spiral as
 *          oriented on the March 1964 cover of Scientific American.
 * https://oeis.org/A168027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n^2 + 3*n + 1 for all n such that this number is prime
 *            * (a(0) = 1). The function enumerates n, tests each value for
 *            primality, * and returns the (n+1)-st qualifying number. The
 *            count is taken via a * linear scan; this is fast enough for the
 *            required bounds and works for * any of the sup...
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 993 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

seq_t a(idx_t n)
{
    idx_t k = 0;           /* current n in the quadratic */
    idx_t cnt = 0;         /* number of qualifying terms seen */

    while (1) {
        /* compute 4*k^2 + 3*k + 1 in the appropriate seq_t type */
        seq_t kseq = (seq_t)k;
        seq_t val = ((seq_t)4) * kseq * kseq + ((seq_t)3) * kseq + 1;

        /* accept 1 (only for k==0) or a prime */
        if (k == 0 || is_prime_big(val)) {
            if (cnt == n)
                return val;
            ++cnt;
        }
        ++k;
    }
}
