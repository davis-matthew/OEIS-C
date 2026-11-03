/*
 * A046360  Composite palindromes divisible by the palindromic sum of their
 *          prime factors (counted with multiplicity).
 * https://oeis.org/A046360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   factor_u64, is_palindrome_base, is_prime, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A046360
   Composite palindromes divisible by the palindromic sum of their
   prime factors (counted with multiplicity).  The function searches
   palindromes in ascending order, factorises each composite, checks
   the sum condition, and returns the n‑th term. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_palindrome_base.h"
#include "../utils/is_prime.h"
#include "../utils/reverse_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1 */

    idx_t found = 0;                      /* number of terms found */
    idx_t length = 1;                     /* current palindrome length */

    /* auxiliary arrays for factorisation (enough for 64‑bit numbers) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count;

    while (found < n) {
        idx_t half_len = (length + 1) / 2;                /* digits in left half */
        seq_t start_val = pow10(half_len - 1);            /* left must have half_len digits */
        seq_t end_val   = pow10(half_len) - 1;            /* inclusive */

        for (seq_t left = start_val; left <= end_val; ++left) {
            seq_t pal, rev, basePow;

            /* build palindrome */
            if (length % 2 == 0) {                        /* even length */
                basePow = pow10(half_len);
                rev = reverse_digits(left);
                pal = left * basePow + rev;
            } else {                                     /* odd length */
                basePow = pow10(half_len - 1);
                rev = reverse_digits(left / (seq_t)10);
                pal = left * basePow + rev;
            }

            /* skip numbers smaller than 4 (no composites) */
            if (pal < 4) continue;

            /* composite test */
            if (is_prime((idx_t)pal)) continue;

            /* factorise (pal is assumed to fit in 64 bits) */
            factor_u64((uint64_t)pal, primes, exps, &count);

            /* compute sum of prime factors with multiplicity */
            seq_t sum_factors = 0;
            for (size_t i = 0; i < count; ++i)
                sum_factors += (seq_t)primes[i] * (seq_t)exps[i];

            /* sum must itself be a palindrome */
            if (!is_palindrome_base(sum_factors, 10)) continue;

            /* palindrome must be divisible by this sum */
            if (sum_factors == 0) continue;          /* safety */
            if (pal % sum_factors == 0) {
                ++found;
                if (found == n) return pal;
            }
        }

        ++length;   /* move to next palindrome length */
    }

    return 0;  /* unreachable */
}
