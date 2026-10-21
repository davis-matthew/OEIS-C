/*
 * A070331  Numbers in which suffixing or inserting a 0 anywhere yields only
 *          composite numbers.
 * https://oeis.org/A070331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   numbers n such that inserting a 0 between digits or at the end
 *            yields only composite numbers (OEIS A070331)
 * Range:     u8 n <= 28, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_len, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

/* Helper: check whether a decimal number satisfies the zero‑insertion
   composite property. */
static bool ok(seq_t x) {
    int d = digit_len(x);               /* number of decimal digits */
    seq_t pow10[64];                    /* 10^k, long enough for all d */
    pow10[0] = 1;
    for (int i = 1; i <= d; ++i) pow10[i] = pow10[i-1] * 10;

    for (int off = 0; off < d; ++off) {
        seq_t left  = x / pow10[d - off - 1];
        seq_t right = x % pow10[d - off - 1];
        seq_t ins   = left * pow10[d - off] + right;  /* insert 0 */
        if (is_prime_big(ins)) return false;          /* a prime appears */
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t found = 0;          /* how many qualifying numbers found so far */
    idx_t cand  = 1;          /* current candidate to test */

    while (found < n) {
        if (ok((seq_t)cand)) ++found;  /* qualifies → count it */
        ++cand;                        /* try next integer */
    }
    return (seq_t)(cand - 1);           /* last found number */
}
