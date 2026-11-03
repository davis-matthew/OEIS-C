/*
 * A141546  Numbers whose abundance is 14.
 * https://oeis.org/A141546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^k * (2^{k+1} - 15), where p = 2^{k+1} - 15 is prime and
 *            k is the n-th integer for which that prime occurs
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-------------------------------------------------------------------------*/
/*  OEIS A141546 – Numbers whose abundance is 14                          */
/*  a(n) = 2^k * (2^{k+1} - 15)   with 2^{k+1} - 15 prime                 */
/*-------------------------------------------------------------------------*/

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
    if (n == 0) return 0;           /* undefined for n == 0 */

    idx_t found = 0;
    for (idx_t k = 4; /* infinite loop */ ; ++k) {
        /* 2^{k+1} - 15  */
        seq_t p = ((seq_t)1 << (unsigned long long)(k + 1)) - (seq_t)15;

        /* skip impossible cases (e.g. p <= 1) */
        if (p <= (seq_t)1) continue;

        if (is_prime_big(p)) {
            ++found;
            if (found == n) {
                /* 2^k * p  */
                return ((seq_t)1 << (unsigned long long)k) * p;
            }
        }
    }
}
