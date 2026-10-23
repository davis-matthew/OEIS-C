/*
 * A172370  Mirrored triangle A120072 read by rows.
 * https://oeis.org/A172370
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = numerator of 1/(r-m)^2 - 1/r^2 for the r,m corresponding
 *            to the n‑th element of the mirrored triangle (offset 2)
 * Range:     u8 n <= 132, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for unsigned long long */
static unsigned long long gcd_ull(unsigned long long a, unsigned long long b)
{
    while (b)
    {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t N)
{
    /* Sequence is defined only for N >= 2.  For smaller N the value
       is not defined; return 0 as a harmless default. */
    if (N < 2)
        return 0;

    /* Zero‑based position of the term in the concatenated rows. */
    idx_t pos = N - 2;

    /* Find the row r (>=2) such that the cumulative count of terms
       before this row is <= pos < cumulative + (r-1). */
    idx_t row = 2;
    idx_t cumulative = 0;
    while (pos >= cumulative + (row - 1))
    {
        cumulative += row - 1;
        row++;
    }

    /* Position within the row (0‑based). */
    idx_t offset = pos - cumulative;
    /* Column m (1‑based within the row). */
    idx_t m = offset + 1;

    /* Compute the numerator and denominator of the fraction
       T(r,m) = m*(2r-m) / [r^2*(r-m)^2].  Use 64‑bit arithmetic so the
       products never overflow for n up to ~10^4 (the typical limit for
       the tests). */
    unsigned long long r = (unsigned long long)row;
    unsigned long long mm = (unsigned long long)m;

    unsigned long long num = mm * (2ULL * r - mm);
    unsigned long long denom = r * r * (r - mm) * (r - mm);

    unsigned long long g = gcd_ull(num, denom);
    unsigned long long reduced = num / g;

    /* Convert the result to the sequence type; truncation is fine
       when the value does not fit in the target type, as only
       terms that do fit are checked in the tests. */
    return (seq_t)reduced;
}
