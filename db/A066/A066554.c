/*
 * A066554  Sum of digits of n-th term of A066553.
 * https://oeis.org/A066554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the decimal digits of A066553(n), i.e. the sum of
 *            the n digits of 1357911131517... starting at digit position
 *            n(n-1)/2+1
 * Range:     u8 n <= 43, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void A066554_digits(uint64_t num, unsigned char *buf, int *len)
{
    unsigned char tmp[24];
    int k = 0;
    do {
        tmp[k++] = (unsigned char)(num % 10);
        num /= 10;
    } while (num > 0);
    for (int i = 0; i < k; ++i)
        buf[i] = tmp[k - 1 - i];
    *len = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t pos = ((uint64_t)n * ((uint64_t)n - 1)) / 2; /* 0-based start */

    /* Locate the block of d-digit odd numbers containing pos. */
    uint64_t d = 1, count = 5, first = 1, p10 = 1;
    while (pos >= count * d) {
        pos -= count * d;
        ++d;
        p10 *= 10;
        count = 45 * (p10 / 10);
        first = p10 + 1;
    }
    uint64_t num = first + 2 * (pos / d);
    int off = (int)(pos % d);

    unsigned char buf[24];
    int len;
    A066554_digits(num, buf, &len);

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        sum += (seq_t)buf[off];
        ++off;
        if (off >= len) {
            num += 2;
            A066554_digits(num, buf, &len);
            off = 0;
        }
    }
    return sum;
}
