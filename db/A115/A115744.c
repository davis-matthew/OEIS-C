/*
 * A115744  Triangular numbers that are the product of 2 palindromes greater
 *          than 1.
 * https://oeis.org/A115744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 77 (2026-11-01)
 * Helpers:   ipow_i32, is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute triangular numbers that are the product of two palindromes > 1
   (search via trial division over pre‑computed palindromes).  a(1)=6. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_palindrome_base.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Pre‑computed list of palindromes (excluding 0) up to 6 decimal digits. */
static seq_t pal_list[2000];
static size_t pal_count = 0;

/* Fill pal_list once. */
static void init_palindromes(void)
{
    if (pal_count > 0) return;          /* already done */

    size_t i = 0;

    /* 1‑digit palindromes (2–9, 1 is excluded later) */
    for (int a = 1; a <= 9; ++a) pal_list[i++] = (seq_t)a;

    /* 2‑digit palindromes: aa */
    for (int a = 1; a <= 9; ++a) pal_list[i++] = (seq_t)a * 10 + a;

    /* 3‑digit: aba */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b)
            pal_list[i++] = (seq_t)a * 100 + b * 10 + a;

    /* 4‑digit: abba */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b)
            pal_list[i++] = (seq_t)a * 1000 + b * 100 + b * 10 + a;

    /* 5‑digit: abcba */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b)
            for (int c = 0; c <= 9; ++c)
                pal_list[i++] = (seq_t)a * 10000 + b * 1000 + c * 100
                        + b * 10 + a;

    /* 6‑digit: abccba */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b)
            for (int c = 0; c <= 9; ++c)
                pal_list[i++] = (seq_t)a * 100000 + b * 10000 + c * 1000
                        + c * 100 + b * 10 + a;

    pal_count = i;
}

/* Return the nth term of A115744 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range */

    init_palindromes();

    size_t found = 0;
    idx_t k = 1;

    while (true) {
        seq_t T = ((seq_t)k) * ((seq_t)k + 1) / 2;
        seq_t sqrtT = floor_sqrt(T);

        for (size_t i = 0; i < pal_count && pal_list[i] <= sqrtT; ++i) {
            seq_t p = pal_list[i];
            if (p <= 1) continue;                     /* skip 1 */
            if (T % p == 0) {
                seq_t q = T / p;
                if (q > (seq_t)1 && is_palindrome_base(q, 10)) {
                    ++found;
                    if (found == n) return T;
                    break;                            /* no more p needed */
                }
            }
        }
        ++k;
    }
}
