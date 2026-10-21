/*
 * A085935  Palindromes in A085934.
 * https://oeis.org/A085935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n-th palindrome such that the number formed by sorting
 *            its digits * in ascending order (with leading zeros kept only in
 *            the sorting process) * is a palindrome of P(s) + s, where P(s)
 *            is the product of the digits * of the sorted number. This
 *            function computes the sequence by * generating pa...
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_palindrome(seq_t x)
{
    /* returns true if x is a decimal palindrome */
    unsigned char d[128];
    int len = 0;
    if (x == 0)
        return true;
    while (x != 0) {
        d[len++] = (unsigned char)(x % 10);
        x /= 10;
    }
    for (int i = 0; i + i < len; ++i)
        if (d[i] != d[len - 1 - i])
            return false;
    return true;
}

static seq_t make_palindrome(seq_t pref, bool odd)
{
    /* construct palindrome from prefix */
    seq_t p = pref;
    seq_t rev = pref;
    if (odd)
        rev /= 10; /* skip the middle digit when mirroring */
    while (rev != 0) {
        p = p * 10 + (rev % 10);
        rev /= 10;
    }
    return p;
}

static bool satisfies(seq_t x)
{
    /* check if x satisfies the property: let s be digits of x sorted ascending,
       then s + prod(s) is a palindrome. */
    unsigned char cnt[10] = {0};
    seq_t t = x;
    while (t != 0) {
        cnt[(int)(t % 10)]++;
        t /= 10;
    }
    seq_t s = 0;
    seq_t prod = 1;
    for (int d = 0; d <= 9; ++d) {
        for (int k = 0; k < cnt[d]; ++k) {
            s = s * 10 + d;
            prod *= (seq_t)d;
        }
    }
    return is_palindrome(s + prod);
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (int len = 1;; ++len) {
        bool odd = (len & 1);
        int half = (len + 1) / 2;
        seq_t start = 1;
        for (int i = 1; i < half; ++i)
            start *= 10;
        if (len == 1)
            start = 1;
        seq_t end = start * 10 - 1;

        for (seq_t pref = start; pref <= end; ++pref) {
            seq_t pal = make_palindrome(pref, odd);
            if (satisfies(pal)) {
                ++found;
                if (found == n)
                    return pal;
            }
        }
    }
}
