/*
 * A129479  Triangle read by rows: A054523 * A097806 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A129479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if k == r * a(n) = φ(r/k) if k | r + φ(r/(k+1)) if
 *            (k+1) | r * where r is the row number satisfying r*(r+1)/2 >= n
 *            and * k = n - r*(r-1)/2 (1‑based column index). * Definition of
 *            φ(m): Euler's totient function (number of integers ≤ m coprime
 *            to m)
 * Range:     u8 n <= 44, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euler's totient, computed by trial division (small m). */
static seq_t phi(idx_t m)
{
    seq_t result = m;
    idx_t n = m;
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= (seq_t)n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Find the smallest r such that r*(r+1)/2 >= n. */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the n-th term of OEIS sequence A129479. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1, return 0 harmlessly */
        return 0;

    /* Determine row r and column k for index n. */
    idx_t r = find_row(n);
    seq_t offset_before = ((seq_t)(r - 1) * (seq_t)r) / 2;
    idx_t k = n - (idx_t)offset_before;          /* 1‑based column */

    /* Edge case: last element in the row. */
    if (k == r)
        return 1;

    seq_t val = 0;

    /* Add φ(r/k) if k divides r. */
    if (r % k == 0) {
        idx_t d = r / k;
        val += phi(d);
    }

    /* Add φ(r/(k+1)) if k+1 divides r. */
    if (k + 1 <= r && r % (k + 1) == 0) {
        idx_t d = r / (k + 1);
        val += phi(d);
    }

    return val;
}
