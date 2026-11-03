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
 * Range:     u8 n <= 32, u16 n <= 1865, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 103 terms through n = 9672 (2026-11-01)
 * Helpers:   is_binary_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_binary_palindrome.h"

/* Compute the nth binary palindrome (1‑based).
   n==1 returns 0. */
static seq_t nth_binpal(idx_t n)
{
    if (n == 1) return (seq_t)0;
    idx_t m = n - 1;          /* exclude 0 */

    unsigned len = 1;
    while (1) {
        unsigned half = (len + 1) / 2;
        idx_t cnt = (idx_t)1 << (half - 1);

        if (m <= cnt) break;
        m -= cnt;
        ++len;
    }

    unsigned half = (len + 1) / 2;
    idx_t offset0 = m - 1;                      /* 0‑based offset within this length */
    seq_t prefix = ((seq_t)1 << (half - 1)) | (seq_t)offset0;

    /* Mirror the prefix to construct the palindrome */
    seq_t rev = 0;
    seq_t tmp = prefix;
    if (len % 2 == 0) {
        for (unsigned i = 0; i < half; ++i) {
            rev = (rev << 1) | (tmp & 1);
            tmp >>= 1;
        }
    } else {
        tmp >>= 1;         /* skip middle bit */
        for (unsigned i = 0; i < half - 1; ++i) {
            rev = (rev << 1) | (tmp & 1);
            tmp >>= 1;
        }
    }
    return (prefix << (len - half)) | rev;
}

/* Rank of a binary palindrome x (1‑based, rank(0)=1). */
static seq_t rank_binpal(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)1;

    /* determine bit length */
    unsigned len = 0;
    seq_t temp = x;
    while (temp) { temp >>= 1; ++len; }

    seq_t count = 1;   /* count includes 0 */

    /* count palindromes of shorter lengths */
    for (unsigned l = 1; l < len; ++l) {
        unsigned half = (l + 1) / 2;
        count += (seq_t)1 << (half - 1);
    }

    /* same length */
    unsigned half = (len + 1) / 2;
    seq_t prefix = x >> (len - half);
    seq_t min_prefix = (seq_t)1 << (half - 1);
    seq_t smaller = prefix - min_prefix;
    count += smaller;

    /* include the current palindrome itself */
    ++count;
    return count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;           /* special case */

    seq_t cur = nth_binpal(n);
    unsigned cnt = 0;

    while (true) {
        if (!is_binary_palindrome(cur)) break;
        if (cur == (seq_t)3) {             /* avoid infinite loop on 3 */
            ++cnt;
            break;
        }
        ++cnt;
        seq_t nxt = rank_binpal(cur);
        if (!is_binary_palindrome(nxt)) {
            break;
        }
        cur = nxt;
    }
    return (seq_t)cnt;
}
