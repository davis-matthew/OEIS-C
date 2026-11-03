/*
 * A181525  Prime numbers such that the difference between any pair of digits
 *          is 0, 2, 3, 5 or 7.
 * https://oeis.org/A181525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by a brute‑force search:
   a(n) is the n-th prime whose every pair of decimal digits differs
   by 0, 2, 3, 5 or 7. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"
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

/* Helper: test whether a prime satisfies the digit difference condition. */
static bool digit_diff_valid(seq_t p)
{
    /* buffer large enough for the expected digit count */
    uint8_t digits[8192];
    size_t len = 0;
    get_digits(p, digits, &len);

    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            int d = (int)digits[i] - (int)digits[j];
            if (d < 0) d = -d;
            if (d != 0 && d != 2 && d != 3 && d != 5 && d != 7)
                return false;
        }
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1 – return 0 */
        return 0;

    /* Handle the first prime specially. */
    if (n == 1)
        return (seq_t)2;

    /* Count of valid primes found so far. */
    idx_t found = 1;          /* prime 2 is already counted */

    /* Start checking with the next odd number. */
    seq_t candidate = 3;

    while (found < n) {
        if (is_prime_big(candidate) && digit_diff_valid(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        candidate += 2;  /* skip even numbers */
    }

    /* Unreachable – the loop always returns. */
    return (seq_t)0;
}
