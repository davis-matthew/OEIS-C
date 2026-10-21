/*
 * A067271  Numbers n such that the digits of P_7(n), the n-th heptagonal
 *          number, end in n.
 * https://oeis.org/A067271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer m such that P_7(m) = m(5m-3)/2
 *            ends with the decimal digits of m
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   append_candidate, digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits_of.h"
#include "../utils/pow10.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static idx_t scan = 1;     /* next candidate to test */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        seq_t m = (seq_t)scan;          /* candidate number */
        int d = digits_of(scan);       /* number of decimal digits of m */
        seq_t mod = pow10((idx_t)d);    /* 10^d as seq_t */
        seq_t p   = m * ((seq_t)5 * m - 3) / 2; /* P_7(m) */

        if (p % mod == m) {             /* last d digits equal m */
            append_candidate(&cache, &len, &cap, m);
        }
        scan++;                          /* move on to next candidate */
    }

    /* Sequence is 1-indexed: a(1) is the first term. */
    return cache[n - 1];
}
