/*
 * A044163  Numbers k such that string 3,4 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose base‑7 representation contains the
   substring "34" but whose predecessor does not.  Brute‑force search
   over successive integers.  */
#include <stdbool.h>

static bool has34(seq_t x)
{
    if (x < 7) return false;          /* need at least two digits */
    seq_t prev = x % 7;
    x /= 7;
    while (x > 0) {
        seq_t cur = x % 7;
        if (cur == 3 && prev == 4) return true;
        prev = cur;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t k = 1;
    while (1) {
        if (has34(k) && !has34(k - 1)) {
            ++cnt;
            if (cnt == n) return k;
        }
        ++k;
    }
}
