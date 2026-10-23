/*
 * A175691  a(n) is the smallest positive integer such that a(n)*n is an
 *          anagram of a(n)*2.
 * https://oeis.org/A175691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

/* Search for the smallest positive integer a(n) such that
 * a(n)*n and a(n)*2 contain the same decimal digits (anagram).
 * 2a(n) and n*a(n) have identical digit multiset.  The search
 * skips values that cannot satisfy the necessary condition
 * (a*(n-2)) ≡ 0 (mod 9) by stepping over multiples of
 * 9/gcd(9,|n-2|). */

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Compute step based on (a*(n-2)) divisible by 9. */
    idx_t d = (n > 2) ? (n - 2) : (2 - n);   /* |n-2| */
    idx_t g = (d == 0) ? 9 : gcd_idx(d, 9);  /* gcd(9,|n-2|) */
    seq_t step = (seq_t)(9 / g);             /* 9/gcd */

    unsigned char cnt1[10], cnt2[10];
    for (seq_t a_val = step; ; a_val += step) {
        seq_t prod1 = a_val * ((seq_t)n);
        seq_t prod2 = a_val * ((seq_t)2);

        digit_counts(prod1, cnt1);
        digit_counts(prod2, cnt2);

        bool equal = true;
        for (int i = 0; i < 10; ++i) {
            if (cnt1[i] != cnt2[i]) {
                equal = false;
                break;
            }
        }
        if (equal) return a_val;
    }
}
