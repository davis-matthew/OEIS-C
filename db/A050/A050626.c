/*
 * A050626  Product of digits of n is a nonzero square.
 * https://oeis.org/A050626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 4838, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) is the n-th positive integer whose decimal digit product
 * is a non‑zero perfect square.
 * Offset: 1
 */

#include <stdbool.h>

/* prime exponents for digits 0‑9 (2, 3, 5, 7) */
static const int digit_exp[10][4] = {
    {0,0,0,0}, /* 0 (unused) */
    {0,0,0,0}, /* 1 */
    {1,0,0,0}, /* 2 */
    {0,1,0,0}, /* 3 */
    {2,0,0,0}, /* 4 */
    {0,0,1,0}, /* 5 */
    {1,1,0,0}, /* 6 */
    {0,0,0,1}, /* 7 */
    {3,0,0,0}, /* 8 */
    {0,2,0,0}  /* 9 */
};

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* should never be called with n < 1 */

    uint64_t count    = 0;
    uint64_t candidate = 1;

    while (true) {
        int e2 = 0, e3 = 0, e5 = 0, e7 = 0;
        bool zero_digit = false;
        uint64_t tmp = candidate;

        /* accumulate prime exponents of the digit product */
        while (tmp > 0) {
            int d = tmp % 10;
            if (d == 0) {          /* product would be zero – reject */
                zero_digit = true;
                break;
            }
            const int *e = digit_exp[d];
            e2 += e[0];
            e3 += e[1];
            e5 += e[2];
            e7 += e[3];
            tmp /= 10;
        }

        /* check if product is a non‑zero perfect square */
        if (!zero_digit &&
            (e2 % 2 == 0) &&
            (e3 % 2 == 0) &&
            (e5 % 2 == 0) &&
            (e7 % 2 == 0)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
