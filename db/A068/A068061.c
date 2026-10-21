/*
 * A068061  Palindromic numbers j that are not of the form k + reverse(k) for
 *          any k.
 * https://oeis.org/A068061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 327, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the n‑th odd‑digit palindrome with odd central digit
 * (OEIS A068061).  All palindromes of odd length whose middle digit is
 * 1,3,5,7, or 9 are produced in increasing order.  Since every such
 * palindrome cannot be written as k + reverse(k), the sequence is exactly
 * these numbers. */

#include <stdint.h>
#include <stdbool.h>

/* powers of ten up to 10^20 (enough for n ≤ 10^4 in this sequence) */
static const unsigned long long POW10[21] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL,
    100000000000000000000ULL
};

/* reverse the decimal representation of an unsigned long long */
static unsigned long long reverse_uint64(unsigned long long x)
{
    unsigned long long r = 0;
    while (x > 0) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

/* required function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    const unsigned long long CENTER[5] = {1ULL, 3ULL, 5ULL, 7ULL, 9ULL};

    for (int m = 0; ; ++m) {          /* m = (length-1)/2, i.e. number of digits left of centre */
        unsigned long long l_start, l_end;
        if (m == 0) {                 /* length = 1, single digit numbers */
            l_start = 0;
            l_end   = 0;
        } else {                       /* left part has m digits, first digit non‑zero */
            l_start = POW10[m - 1];
            l_end   = POW10[m] - 1;
        }

        for (unsigned long long l = l_start; l <= l_end; ++l) {
            unsigned long long rev_l = reverse_uint64(l);
            for (int i = 0; i < 5; ++i) {
                unsigned long long d = CENTER[i];
                seq_t pal = (seq_t)l * POW10[m + 1]
                            + (seq_t)d * POW10[m]
                            + (seq_t)rev_l;
                ++count;
                if (count == n)
                    return pal;
            }
        }
    }
}
