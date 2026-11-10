/*
 * A206439  Partial sums of A064692.
 * https://oeis.org/A206439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0..n} A064692(k), where A064692(0)=1 and
 *            A064692(k) = number of holes in the decimal digits of k (0,4,6,9
 *            count 1; 8 counts 2). Computed by direct summation
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char A064692_digit_holes[10] = {1, 0, 0, 0, 1, 0, 1, 0, 2, 1};

static seq_t A064692_a(idx_t n)
{
    if (n == 0) return 1;
    seq_t total = 0;
    while (n > 0)
    {
        total += A064692_digit_holes[n % 10];
        n /= 10;
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    for (k = 0; k <= n; k++)
    {
        s += A064692_a(k);
        if (k == n) break;
    }
    return s;
}
