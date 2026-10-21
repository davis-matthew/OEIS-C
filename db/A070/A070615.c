/*
 * A070615  Minimal difference between any two digits of n in base 10.
 * https://oeis.org/A070615
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal difference between any two decimal digits of n.
   If any digit occurs at least twice the answer is 0.
   Otherwise the answer is the smallest absolute difference between
   distinct digits that appear in n. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* use seq_t for all calculations */

    int freq[10] = {0};
    /* Scan all decimal digits of n */
    while (x != 0) {
        int d = (int)(x % 10);   /* digit 0‑9 (seq_t -> int) */
        if (++freq[d] == 2)      /* duplicate digit found */
            return (seq_t)0;
        x /= 10;
    }

    /* Gather the digits that actually appear */
    int min_diff = 10;           /* larger than any possible difference */
    for (int i = 0; i < 10; ++i) {
        if (freq[i] == 0) continue;
        for (int j = i + 1; j < 10; ++j) {
            if (freq[j] == 0) continue;
            int diff = j - i;
            if (diff < min_diff)
                min_diff = diff;
        }
    }

    /* For n ≥ 10 this must always produce a valid digit difference. */
    if (min_diff == 10)
        return (seq_t)0;
    return (seq_t)min_diff;
}
