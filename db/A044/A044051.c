/*
 * A044051  a(n) = (s(n+1) + 1)/2, where s = A006995 (base-2 palindromes).
 * https://oeis.org/A044051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+1)/2 where s(n) is the nth base‑2 palindrome
 *            (A006995)
 * Range:     u8 n <= 30, u16 n <= 764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9726 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t nth_binary_palindrome(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t rem = n;
    int L = 1;          /* current palindrome length */
    while (1) {
        int half = (L + 1) >> 1;         /* ceil(L/2) */
        unsigned long long cnt = 1ULL << (half - 1);
        if (rem <= cnt)
            break;
        rem -= cnt;
        ++L;
    }

    int half   = (L + 1) >> 1;           /* ceil(L/2) */
    unsigned long long idx0 = rem - 1;   /* 0‑based index within this length */
    unsigned long long leftHalf = (1ULL << (half - 1)) | idx0; /* leading bit is 1 */

    int shift = L - half;                /* floor(L/2) */

    unsigned long long tmp = leftHalf;
    if (L & 1)                /* skip middle bit for odd length */
        tmp >>= 1;
    unsigned long long right = 0;
    while (tmp) {
        right = (right << 1) | (tmp & 1);
        tmp >>= 1;
    }

    return ((seq_t)leftHalf << shift) | (seq_t)right;
}

seq_t a(idx_t n)
{
    seq_t pal = nth_binary_palindrome(n);
    return (pal + 1) / 2;
}
