/*
 * A098943  Numbers where 6 is the only even decimal digit.
 * https://oeis.org/A098943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 3300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098943: Numbers whose decimal digits are 1,3,5,6,7,9
   and which contain at least one 6.  The nth term is the k‑th
   lexicographically smallest length‑L string over that alphabet
   that contains a 6, where L is the smallest integer with
   (6^L-5^L) + sum_{i< L}(6^i-5^i) ≥ n. */
#include <stdbool.h>

static inline idx_t pow_idx(idx_t base, idx_t exp)
{
    idx_t res = 1;
    while (exp--)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0 */

    /* Find the length L of the number */
    idx_t cum = 0;     /* cumulative count of numbers of shorter lengths */
    idx_t L = 0;
    while (1) {
        ++L;
        idx_t countL = pow_idx(6, L) - pow_idx(5, L);
        if (cum + countL >= n)
            break;
        cum += countL;
    }

    idx_t k = n - cum;  /* 1‑based index within numbers of length L */

    const int allowed[6] = {1, 3, 5, 6, 7, 9};
    bool used6 = false;
    seq_t value = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        int rem = (int)(L - pos - 1);  /* remaining positions */
        for (int idx = 0; idx < 6; ++idx) {
            int digit = allowed[idx];
            bool newUsed6 = used6 || (digit == 6);

            idx_t count;
            if (newUsed6) {
                count = pow_idx(6, rem);
            } else {
                count = pow_idx(6, rem) - pow_idx(5, rem);
            }

            if (k > count) {
                k -= count;
            } else {
                value = value * (seq_t)10 + (seq_t)digit;
                used6 = newUsed6;
                break;
            }
        }
    }

    return value;
}
