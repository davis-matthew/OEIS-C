/*
 * A043082  (s(n)+7)/10, where s(n)=n-th base 10 palindrome that starts with
 *          3.
 * https://oeis.org/A043082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+7)/10, where s(n) is the n-th base 10 palindrome
 *            that starts with 3
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pal_count_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pal_count_len.h"

static seq_t reverse_digits(seq_t x)
{
    seq_t rev = (seq_t)0;
    while (x > (seq_t)0) {
        rev = rev * (seq_t)10 + (x % (seq_t)10);
        x = x / (seq_t)10;
    }
    return rev;
}

static seq_t pow10_seq_t(idx_t exp)
{
    seq_t v = (seq_t)1;
    while (exp--) v *= (seq_t)10;
    return v;
}

seq_t a(idx_t n)
{
    /* Find the length of the n-th palindrome starting with 3 */
    idx_t len = (idx_t)1;
    idx_t cumulative = (idx_t)0;

    while (1) {
        idx_t cnt = pal_count_len(len);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++len;
    }

    idx_t half_len = (len + (idx_t)1) / (idx_t)2;
    idx_t offset = n - cumulative - (idx_t)1;            /* zero‑based index within this length */

    seq_t pow10_half_minus1 = pow10_seq_t(half_len - (idx_t)1);
    seq_t half = (seq_t)3 * pow10_half_minus1 + (seq_t)offset;

    seq_t pal;
    if ((len & (idx_t)1) == (idx_t)0) { /* even length */
        seq_t pow10_half = pow10_seq_t(half_len);
        pal = half * pow10_half + reverse_digits(half);
    } else {                           /* odd length */
        pal = half * pow10_half_minus1 + reverse_digits(half / (seq_t)10);
    }

    return (pal + (seq_t)7) / (seq_t)10;
}
