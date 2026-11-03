/*
 * A177688  Numbers n such that (n+2)//n - (n+1) is prime, where // represents
 *          the concatenation of decimals.
 * https://oeis.org/A177688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth integer n ≥ 0 such that (n+2)*10^k – 1 is prime
 *            where k = number of decimal digits of n
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t N) {
    /* N is 1‑based index.  We iterate over non‑negative integers and
       count how many satisfy the prime condition.  When the count
       reaches N we have found the required term. */
    idx_t found = 0;
    for (idx_t cand = 0; ; ++cand) {
        seq_t cand_seq = (seq_t)cand;

        /* Number of decimal digits of cand.  If cand == 0, digit_len
           returns 1, which is correct because 0 has one digit. */
        int k = num_digits(cand_seq);

        /* 10^k, as a seq_t value. */
        seq_t pow10k = pow10((idx_t)k);

        /* The candidate prime value p = (n+2)*10^k – 1. */
        seq_t p = (cand_seq + 2) * pow10k - 1;

        /* Test primality.  The utility handles all the bit‑widths
           that seq_t can represent. */
        if (is_prime_big(p)) {
            ++found;
            if (found == N) {
                return cand_seq;
            }
        }
    }
}
