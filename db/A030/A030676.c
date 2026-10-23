/*
 * A030676  Smallest nontrivial extension of n-th palindrome which is a
 *          square.
 * https://oeis.org/A030676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest square that extends the n‑th decimal palindrome
 *            by at least one digit. The palindrome is taken in natural order
 *            starting at 1: 1,2,…,9,11,22,…,99,101,111,… etc
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 36, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, pow10, reverse_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/reverse_int.h"

static seq_t ceil_sqrt(seq_t x)
{
    seq_t s = floor_sqrt(x);
    if (s * s < x)
        ++s;
    return s;
}

/* Return the n‑th (1‑based) decimal palindrome. */
static seq_t nth_palindrome(idx_t n)
{
    idx_t remaining = n;
    int L = 1;
    while (1) {
        idx_t cnt;
        if (L == 1)
            cnt = 9;
        else {
            int half_len = (L + 1) / 2;
            cnt = 9 * (idx_t)pow10(half_len - 1);
        }
        if (remaining > cnt) {
            remaining -= cnt;
            ++L;
        } else {
            break;
        }
    }

    idx_t offset = remaining - 1;
    seq_t pal;

    if (L == 1) {
        pal = offset + 1;
    } else if (L == 2) {
        seq_t d = offset + 1;
        pal = d * 10 + d;
    } else {
        int half_len = (L + 1) / 2;
        seq_t half_base = pow10(half_len - 1);
        seq_t H = half_base + offset;

        seq_t rev;
        if (L % 2 == 0) {
            rev = reverse_int(H, half_len);
            pal = H * pow10(half_len) + rev;
        } else {
            rev = reverse_int(H / 10, half_len - 1);
            pal = H * pow10(half_len - 1) + rev;
        }
    }
    return pal;
}

seq_t a(idx_t n)
{
    seq_t p = nth_palindrome(n);
    for (idx_t k = 1; ; ++k) {
        seq_t tenk = pow10(k);
        seq_t lower = p * tenk;
        seq_t upper = (p + 1) * tenk - 1;

        seq_t m_min = ceil_sqrt(lower);
        seq_t m_max = floor_sqrt(upper);

        if (m_min <= m_max)
            return m_min * m_min;
    }
}
