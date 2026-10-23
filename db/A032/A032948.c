/*
 * A032948  Numbers k whose base-13 representation Sum_{i=0..m} d(i)*13^(m-i)
 *          has d(i)=0 for all odd i. Here m is the position of the leading
 *          bit of k.
 * https://oeis.org/A032948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base-13 representation has d(i)=0 for all
 *            odd i i.e. in ∑_{i=0..m} d(i)*13^(m-i), every digit at an odd
 *            position from the left must be 0
 * Range:     u8 n <= 37, u16 n <= 674, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static bool is_valid(seq_t val)
{
    seq_t tmp = val;
    unsigned char digits[32]; /* sufficient for reasonably sized numbers */
    int len = 0;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 13);
        tmp /= 13;
    }
    if (len == 0) return false; /* val==0 never queried */
    int L = len;
    for (int r = 0; r < L; ++r) {
        /* r is the position from the right (0‑based).
           The corresponding left position is i = L-1-r.
           If i is odd, r and L-1 must have different parity. */
        if (((L - 1) ^ r) & 1) {
            if (digits[r] != 0)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;
    while (count < n) {
        if (is_valid(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
    return 0; /* Unreachable, keeps compiler happy */
}
