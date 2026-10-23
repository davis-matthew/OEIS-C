/*
 * A187487  Numerator of n minus n-th harmonic number.
 * https://oeis.org/A187487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = n - H_n, where H_n is the n‑th harmonic number. The
 *            returned value is the numerator of the reduced fraction n - H_n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simple Euclidean algorithm for any seq_t type. */
static seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety, not required by spec */

    /* cumulative sum of 1/i as a reduced fraction sum_num/sum_den */
    seq_t sum_num = 0;
    seq_t sum_den = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kseq = (seq_t)k;
        seq_t new_num = sum_num * kseq + sum_den;
        seq_t new_den = sum_den * kseq;
        seq_t g = _gcd_seq(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* Compute n - sum = (n*sum_den - sum_num) / sum_den  */
    seq_t diff_num = (seq_t)n * sum_den - sum_num;
    seq_t diff_den = sum_den;
    seq_t g = _gcd_seq(diff_num, diff_den);
    return diff_num / g;
}
