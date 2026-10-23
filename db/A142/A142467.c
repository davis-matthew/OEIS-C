/*
 * A142467  Triangle T(n,m) read by rows: T(n,m) = Product_{i=0..6}
 *          binomial(n+i,m)/binomial(m+i,m).
 * https://oeis.org/A142467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Π_{i=0..6} binomial(row+i, col) / binomial(col+i, col),
 *            where the triangle is read row‑by‑row, row = ⌊(√(8n+1)-1)/2⌋,
 *            col = n - row(row+1)/2
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 58, u64 n <= 142,
 *            u128 n <= 390, big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"          /* provides gcd(seq_t, seq_t) */

static idx_t row_of(idx_t n) {
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t row = row_of(n);
    __int128 tri = (__int128)row * (row + 1) / 2;
    idx_t col = n - (idx_t)tri;

    /* Store numerator and denominator terms */
    seq_t num[7];
    seq_t den[7];
    for (idx_t i = 0; i < 7; ++i) {
        num[i] = binomial(row + i, col);
        den[i] = binomial(col + i, col);
    }

    /* Cancel common factors between numerators and denominators */
    for (idx_t i = 0; i < 7; ++i) {
        seq_t d = den[i];
        if (d == 1) continue;
        for (idx_t j = 0; j < 7; ++j) {
            seq_t nval = num[j];
            if (nval == 1) continue;
            seq_t g = gcd(d, nval);
            if (g > 1) {
                d /= g;
                num[j] /= g;
                if (d == 1) break;
            }
        }
        den[i] = d;          /* should be 1 after cancellations */
    }

    /* Multiply remaining numerators (denominators should all be 1) */
    seq_t val = 1;
    for (idx_t i = 0; i < 7; ++i) {
        val *= num[i];
    }
    return val;
}
