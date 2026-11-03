/*
 * A055058  Primes in A054632.
 * https://oeis.org/A055058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function returns the nth term of A055058.
 *
 * Algorithm:
 *   • Concatenate the decimal representations of the natural numbers 0,1,2,...
 *   • Keep a running sum of the digits seen so far.
 *   • Whenever the running sum is prime, record it.
 *   • If the digit that produced the sum is 0, the sum does not change;
 *     in that case the same prime would be recorded twice, so we skip
 *     recording when the digit is 0 to avoid duplicates.
 *   • Continue until the requested index n is reached.
 */
#include <stdbool.h>
#include <stddef.h>
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
    if (n == 0) {                /* offset is 1; a(1) is the first term */
        return 0;
    }

    seq_t sum = 0;                /* running digit sum */
    idx_t found = 0;              /* how many primes seen so far */
    idx_t num = 0;                /* current natural number to process */

    /* buffer large enough for the decimal digits of any 64‑bit integer */
    uint8_t digits[65];           
    size_t dcnt;

    while (found < n) {
        /* extract digits of the current number into `digits` reversed */
        if (num == 0) {
            dcnt = 1;
            digits[0] = 0;
        } else {
            dcnt = 0;
            idx_t tmp = num;
            while (tmp > 0) {
                digits[dcnt++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            }
        }

        /* traverse digits most significant → least significant */
        for (size_t i = dcnt; i-- > 0; ) {
            uint8_t digit = digits[i];
            sum += (seq_t)digit;
            if (digit != 0 && is_prime_big(sum)) {
                ++found;
                if (found == n) {
                    return sum;
                }
            }
        }
        ++num;                     /* next integer */
    }

    /* unreachable, but keep compiler happy */
    return sum;
}
