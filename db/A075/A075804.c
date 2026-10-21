/*
 * A075804  Differences between adjacent palindromic nonsquarefree numbers
 *          A035132.
 * https://oeis.org/A075804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the difference between consecutive palindromic
 *            non‑squarefree numbers. That is, a(n) = P(n+1) – P(n) where P(k)
 *            is the k‑th palindromic number that is not squarefree
 * Range:     u8 n <= 13, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_squarefree, pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/reverse.h"         /* seq_t reverse(seq_t v); */
#include "../utils/pow10.h"           /* seq_t pow10(idx_t e); */
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t x); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1: undefined for n==0 */

    seq_t prev = 0;          /* previous palindromic non‑squarefree number */
    idx_t diff_idx = 0;      /* number of differences already produced */

    /* Generate palindromic numbers in increasing order until we have
       produced the n‑th difference. */
    for (idx_t len = 1; diff_idx < n; ++len) {          /* length of palindrome */
        idx_t half_len = (len + 1) / 2;                 /* digits in the left half */

        /* left half ranges from 10^(half_len-1) to 10^half_len-1 */
        uint64_t start = 1;
        for (idx_t i = 1; i < half_len; ++i) start *= 10;
        uint64_t end   = start * 10ULL - 1ULL;

        for (uint64_t left = start; left <= end; ++left) {
            seq_t pal;
            seq_t left_val = (seq_t)left;

            if (len % 2 == 1) {        /* odd length palindrome */
                seq_t rev = reverse(left_val / 10);
                pal = left_val * pow10(half_len - 1) + rev;
            } else {                   /* even length palindrome */
                seq_t rev = reverse(left_val);
                pal = left_val * pow10(half_len) + rev;
            }

            if (!is_squarefree(pal)) {
                if (prev != 0) {
                    seq_t diff = pal - prev;
                    if (diff_idx + 1 == n) {
                        return diff;     /* found the desired difference */
                    }
                    ++diff_idx;
                }
                prev = pal;
            }
        }
    }

    /* The sequence is infinite; code should never reach here. */
    return 0;
}
