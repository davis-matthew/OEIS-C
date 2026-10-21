/*
 * A083870  Number of pairs (x,y) of palindromes such that n-th decimal
 *          palindrome = x*y and 1<=x<=y.
 * https://oeis.org/A083870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of unordered pairs (x,y) of decimal palindromes
 *            such that the n‑th decimal palindrome = x * y and 1 ≤ x ≤ y
 * Range:     u8 n <= 14, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_sqrt, is_palindrome, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_palindrome.h"

/* Return the n-th decimal palindrome (n ≥ 1). */
static seq_t nth_palindrome(idx_t n)
{
    idx_t counted = 0;          /* palindromes already accounted for */
    for (idx_t len = 1; ; ++len) {
        idx_t half = (len + 1) / 2;              /* number of free digits */
        seq_t first = pow10(half - 1);           /* 10^(half-1) */
        idx_t block = (idx_t)(9 * first);        /* palindromes of this length */

        if (counted + block >= n) {              /* target lies here */
            idx_t offset = n - counted - 1;      /* zero based offset in block */
            seq_t prefix = first + (seq_t)offset;
            seq_t pal = prefix;
            seq_t rev = prefix;
            if (len & 1)                         /* odd length → skip middle digit */
                rev /= (seq_t)10;
            while (rev > 0) {
                pal = pal * (seq_t)10 + (rev % (seq_t)10);
                rev /= (seq_t)10;
            }
            return pal;
        }
        counted += block;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; undefined for n<1 */

    seq_t P = nth_palindrome(n);              /* n-th palindrome */
    seq_t sqrt_seq = floor_sqrt(P);
    idx_t sqrtP = (idx_t)sqrt_seq;            /* sqrtP fits in idx_t */

    seq_t count = 0;
    for (idx_t d = 1; d <= sqrtP; ++d) {
        seq_t d_seq = (seq_t)d;
        if (P % d_seq == 0) {
            seq_t e_seq = P / d_seq;
            if (is_palindrome(d_seq, (idx_t)10) &&
                is_palindrome(e_seq, (idx_t)10))
            {
                ++count;                      /* d ≤ e automatically */
            }
        }
    }
    return count;
}
