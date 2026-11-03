/*
 * A069851  Number of distinct primes obtained by inserting a 0 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069851: a(n) = number of distinct primes obtained by inserting a
 * 0 in every possible decimal position of the integer n (including
 * prefixing). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits_idx.h"
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

seq_t a(idx_t n)
{
    if (n < 1)            /* offset is 1; below 1 is irrelevant */
        return 0;

    /* Number of decimal digits of n */
    idx_t len = num_digits_idx(n);          /* digits(n) returns int, assigned to idx_t */

    /* At most len+1 insertion positions */
    idx_t pos_count = len + 1;
    seq_t primes[pos_count];
    size_t uniq = 0;

    /* For each position, build the number with a zero inserted */
    for (idx_t pos = 0; pos <= len; ++pos) {
        idx_t digits_after = len - pos;          /* number of digits to the right of the insertion point */

        seq_t pow_shift = pow10(digits_after + 1);   /* 10^(digits_after+1) */
        seq_t pow_after = pow10(digits_after);       /* 10^digits_after */

        /* Split n into left and right parts */
        seq_t left  = (seq_t)n / pow_after;
        seq_t right = (seq_t)n % pow_after;

        /* Construct the new number: left part + 0 + right part */
        seq_t new_val = left * pow_shift + right;

        /* If it is a prime, count it if not already counted */
        if (is_prime_big(new_val)) {
            bool seen = false;
            for (size_t k = 0; k < uniq; ++k) {
                if (primes[k] == new_val) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                primes[uniq++] = new_val;
            }
        }
    }

    return (seq_t)uniq;
}
