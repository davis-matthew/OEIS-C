/*
 * A206921  Rank of the n-th binary palindrome. The minimal number of
 *          iterations A206915(A206915(...A206915(A006995(n))...)) such that
 *          the result is not a binary palindrome, a(3)=1.
 * https://oeis.org/A206921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal k such that after k applications of the
 *            rank‑of‑binary‑palindrome map to the nth binary palindrome, the
 *            result is not a binary palindrome. Recurrence: a(3)=1; for n>1:
 *            a(n)=1 if A006995(n) not palindrome; else
 *            a(n)=a(rank(A006995(n)))+1 (special case handled)
 * Range:     u8 n <= 32, u16 n <= 1865, u32 n <= 9672, u64 n <= 9672,
 *            u128 n <= 9672, big any n
 * Helpers:   is_pal_binary, nth_binpal, rank_binpal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_pal_binary.h"
#include "../utils/nth_binpal.h"
#include "../utils/rank_binpal.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;           /* special case */

    seq_t cur = nth_binpal(n);
    unsigned cnt = 0;

    while (true) {
        if (!is_pal_binary(cur)) break;
        if (cur == (seq_t)3) {             /* avoid infinite loop on 3 */
            ++cnt;
            break;
        }
        ++cnt;
        seq_t nxt = rank_binpal(cur);
        if (!is_pal_binary(nxt)) {
            break;
        }
        cur = nxt;
    }
    return (seq_t)cnt;
}
