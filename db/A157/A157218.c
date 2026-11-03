/*
 * A157218  Number of ways to write the n-th positive odd integer in the form
 *          p+2^x+7*2^y with p a prime congruent to 1 mod 6 and x,y positive
 *          integers.
 * https://oeis.org/A157218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 199999, u32 n <= 200000, u64 n <= 200000,
 *            u128 n <= 200000, big any n
 * Validated: standard, pass, 66 terms through n = 9487 (2026-11-03)
 * Helpers:   is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation of OEIS sequence A157218.
 * a(n) = number of representations of 2*n‑1 as
 *        p + 2^x + 7*2^y
 *        where p is prime ≡ 1 (mod 6) and x,y ≥ 1.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
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
    /* target odd integer 2n‑1 */
    seq_t target = ((seq_t)2 * n) - 1;
    seq_t count  = 0;

    /* loop over x with 2^x < target‑1  (so that p>0) */
    for (idx_t x = 1;; ++x)
    {
        seq_t powx = pow2(x);
        if (powx > target - 1) break;

        /* loop over y with 7*2^y < target‑1 */
        for (idx_t y = 1;; ++y)
        {
            seq_t powy       = pow2(y);
            seq_t seven_powy = (seq_t)7 * powy;
            if (seven_powy > target - 1) break;

            seq_t sum = powx + seven_powy;
            if (sum >= target) break;      /* sum grows with y */

            seq_t p = target - sum;
            if (p % 6 == 1 && is_prime_big(p))
                ++count;
        }
    }
    return count;
}
