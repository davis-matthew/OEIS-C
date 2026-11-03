/*
 * A028426  Clog sequence in base 5. Right to left concatenation of
 *          n,int(log_5(n)),int(log_5(int(log_5(n)))),... in base5.
 * https://oeis.org/A028426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = right‑to‑left concatenation in base 5 of n, ⌊log₅(n)⌋,
 *            ⌊log₅(⌊log₅(n)⌋)⌋, …, 0
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Number of base‑5 digits of x (x ≥ 0).  0 has one digit. */
static size_t
digits_base5(idx_t x)
{
    if (x == 0)
        return 1;
    size_t d = 0;
    while (x) {
        x /= 5;
        ++d;
    }
    return d;
}

/* ⌊log₅(x)⌋ for x ≥ 0. */
static idx_t
floor_log5(idx_t x)
{
    if (x < 5)
        return 0;
    idx_t r = 0;
    while (x >= 5) {
        x /= 5;
        ++r;
    }
    return r;
}

/* Small pre‑computed powers of 5 (5⁰ .. 5⁶⁴). */
static seq_t
power_of_5(size_t k)
{
    static seq_t pow5[65];
    static int built = 0;
    if (!built) {
        pow5[0] = 1;
        for (size_t i = 1; i < sizeof(pow5) / sizeof(pow5[0]); ++i)
            pow5[i] = pow5[i - 1] * 5;
        built = 1;
    }
    return pow5[k];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of sequence domain; harmless */
    /* Build the log chain: n, ⌊log₅(n)⌋, …, 0. */
    idx_t chain[64];
    size_t cnt = 0;
    idx_t tmp = n;
    for (;;) {
        chain[cnt++] = tmp;
        if (tmp == 0)
            break;
        tmp = floor_log5(tmp);
    }
    /* Concatenate from 0 up to n (right‑to‑left). */
    seq_t result = 0;
    for (size_t idx = cnt; idx > 0; --idx) {
        idx_t m = chain[idx - 1];
        size_t d = digits_base5(m);
        result = result * power_of_5(d) + (seq_t)m;
    }
    return result;
}
