/*
 * A087558  a(n) = largest prime that is the concatenation of two numbers
 *          whose product is n.
 * https://oeis.org/A087558
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 70 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087558:  a(n) = largest prime that is the concatenation of
   two factors of n‑1.  For a given n (n ≥ 2) we find every pair
   (a,b) such that a·b = n‑1, form the two possible concatenations
   a‖b and b‖a, test those for primality, and return the maximum.
   If no such prime exists, 0 is returned. */
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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
    /* a(2) is the first defined term; for n < 2 the value is undefined. */
    if (n < 2)
        return 0;

    idx_t m = n - 1;          /* product of the two factors */
    seq_t best = 0;

    /* Iterate over all divisors of m */
    for (idx_t d = 1; d * d <= m; ++d)
    {
        if (m % d != 0)
            continue;

        idx_t e = m / d;          /* complementary divisor */

        /* Concatenate d and e, then e and d */
        seq_t cand1 = concat_parts(d, e);
        seq_t cand2 = concat_parts(e, d);

        if (is_prime_big(cand1) && cand1 > best)
            best = cand1;
        if (is_prime_big(cand2) && cand2 > best)
            best = cand2;
    }

    return best;
}
