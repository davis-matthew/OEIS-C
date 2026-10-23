/*
 * A166387  a(n) = sum (floor (j^2/n), 1 <= j <= n-1) - floor ((n-1)(n-2)/3),
 *          n >= 2.
 * https://oeis.org/A166387
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            \sum_{j=1}^{\,n-1}\!\left\lfloor\dfrac{j^{2}}{n}\right\rfloor *
 *            - \left\lfloor\dfrac{(n-1)(n-2)}{3}\right\rfloor , n >= 2 * *
 *            Computes OEIS sequence A166387. The sum is evaluated directly in
 *            O(n) * time. Because all intermediate values are non‑negative,
 *            integer * division in C provides the...
 * Range:     u8 n <= 336, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t sum = 0;
    seq_t nn  = (seq_t)n;

    for (idx_t j = 1; j < n; ++j) {
        /* floor(j^2 / n)  ->  (seq_t)j * j / nn  */
        sum += ((seq_t)j * j) / nn;
    }

    /* floor((n-1)(n-2) / 3)  ->  ((seq_t)(n-1) * (n-2)) / 3  */
    seq_t sub = ((seq_t)(n - 1) * (seq_t)(n - 2)) / (seq_t)3;

    return sum - sub;
}
