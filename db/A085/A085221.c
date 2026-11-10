/*
 * A085221  Factorial expansions of the entries in A085219.
 * https://oeis.org/A085221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, fini
 * Formula:   a(n) = decimal number whose digits are the factorial-base
 *            expansion of A085219(n) (a(0)=0)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static void A085219_factorial_digits(idx_t n, idx_t digits[32], idx_t *len)
{
    idx_t i = 1;
    *len = 0;
    while (n > 0) {
        digits[*len] = n % (i + 1);
        n /= (i + 1);
        (*len)++;
        i++;
    }
}

static void A085219_index_to_xy(idx_t n, idx_t *x, idx_t *y)
{
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        d++;
    idx_t prev = d * (d + 1) / 2;
    idx_t pos = n - prev;
    *x = d - pos;
    *y = pos;
}

static seq_t A085219_fac[41];
static void A085219_init_factorials(void)
{
    static bool once = false;
    if (once) return;
    once = true;
    A085219_fac[0] = 1;
    A085219_fac[1] = (seq_t)1;
    for (idx_t i = 2; i <= 40; ++i)
        A085219_fac[i] = A085219_fac[i - 1] * (seq_t)i;
}

static seq_t A085219_A_xy(idx_t x, idx_t y)
{
    idx_t dx, dy;
    idx_t digits_x[32], digits_y[32];

    A085219_factorial_digits(x, digits_x, &dx);
    A085219_factorial_digits(y, digits_y, &dy);

    A085219_init_factorials();

    seq_t msd = 0;
    if (dy > 0)
        msd = (seq_t)digits_y[dy - 1];

    seq_t result = 0;
    for (idx_t pos = 1; pos <= dy; ++pos) {
        result += (seq_t)digits_y[pos - 1] * A085219_fac[pos];
    }
    for (idx_t pos = 1; pos <= dx; ++pos) {
        seq_t coeff = (seq_t)digits_x[pos - 1] + msd;
        result += coeff * A085219_fac[dy + pos];
    }
    return result;
}

static seq_t A085219_a(idx_t n)
{
    idx_t x, y;
    A085219_index_to_xy(n, &x, &y);
    return A085219_A_xy(x, y);
}

seq_t a(idx_t n)
{
    seq_t v = A085219_a(n);
    seq_t r = 0;
    seq_t pw = 1;
    seq_t i = 2;
    while (v > 0) {
        r += (v % i) * pw;
        v /= i;
        pw *= 10;
        i++;
    }
    return r;
}
