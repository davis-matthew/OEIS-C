/*
 * A178775  Smallest prime factors of zerofull restricted pandigital numbers.
 * https://oeis.org/A178775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base
 * Formula:   a(n) = A020639(A050278(n)) = (A050278(n) even? 2 : 3)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* factorial table up to 10! (10! = 3628800 > 3265920) */
static const idx_t fact[11] = {1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL,
                               720ULL, 5040ULL, 40320ULL, 362880ULL,
                               3628800ULL};

/* external helper (unused – kept for completeness) */
extern seq_t _smallest_prime_factor(uint64_t n);

seq_t a(idx_t n)
{
    /* n must be >= 1 – for n == 0 a(0) is undefined, return 0 safely */
    if (n == 0) return 0;

    /* zero‑based index of the desired permutation */
    idx_t k = n - 1;

    /* Determine the first digit (1…9) */
    idx_t first_index = k / fact[9];
    uint8_t first_digit = 1 + (uint8_t)first_index;

    /* Remainder for the remaining 9 digits */
    idx_t rem = k % fact[9];

    /* List of the remaining digits in ascending order */
    uint8_t rem_d[9];
    size_t sz = 0;
    for (uint8_t d = 0; d <= 9; ++d) {
        if (d != first_digit) rem_d[sz++] = d;
    }

    /* Identify the last digit of the 10‑digit pandigital number */
    uint8_t last_digit = 0;
    for (size_t pos = 0; pos < 9; ++pos) {
        idx_t f = fact[8 - pos];             /* (9‑pos‑1)! */
        idx_t idx = rem / f;
        rem %= f;
        uint8_t dig = rem_d[idx];

        /* remove used digit from the list */
        for (size_t j = idx; j + 1 < sz; ++j) {
            rem_d[j] = rem_d[j + 1];
        }
        --sz;

        if (pos == 8) last_digit = dig;     /* last of the 9 remaining digits */
    }

    /* The smallest prime factor is 2 if the number is even, else 3 */
    return (last_digit & 1) == 0 ? ((seq_t)2) : ((seq_t)3);
}
