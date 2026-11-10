/*
 * A066584  Product of nonzero digits of A066553(n).
 * https://oeis.org/A066584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = product of nonzero digits of A066553(n), where A066553(n)
 *            is the n-digit block of 1357911131517... (concatenated odd
 *            numbers) starting at digit n(n-1)/2+1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    if (n == 0) return prod;

    uint64_t pos = ((uint64_t)n * ((uint64_t)n - 1)) / 2; /* 0-based digit index */

    /* find digit-length group */
    uint64_t d = 1;
    uint64_t count = 5;       /* number of d-digit odd numbers */
    uint64_t first = 1;       /* first d-digit odd number */
    uint64_t p10 = 1;         /* 10^(d-1) */
    for (;;) {
        uint64_t total = count * d;
        if (pos < total) break;
        pos -= total;
        d++;
        p10 *= 10;
        first = p10 + 1;
        count = 45 * (p10 / 10);
    }
    uint64_t num = first + 2 * (pos / d);
    uint64_t off = pos % d;

    uint64_t remaining = (uint64_t)n;
    while (remaining > 0) {
        unsigned char dig[24];
        int len = 0;
        uint64_t v = num;
        do {
            dig[len++] = (unsigned char)(v % 10);
            v /= 10;
        } while (v > 0);
        /* dig[len-1] is most significant */
        for (int i = len - 1 - (int)off; i >= 0 && remaining > 0; --i) {
            if (dig[i] != 0) prod *= (seq_t)dig[i];
            remaining--;
        }
        off = 0;
        num += 2;
    }
    return prod;
}
