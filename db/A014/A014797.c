/*
 * A014797  Squares of odd square pyramidal numbers.
 * https://oeis.org/A014797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (sum_{k=1}^{m} k^2)^2 where m = 4*(n/2)+1 if n is even,
 *            otherwise m = 4*(n/2)+2. The sum of squares up to m is the n‑th
 *            odd square pyramidal number, and the function returns its square
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the index m of the n-th odd square pyramidal number. */
    idx_t k = n / 2;                      /* block number */
    idx_t m = (n & 1) ? (4 * k + 2) : (4 * k + 1);

    /* Compute S(m) = m*(m+1)*(2*m+1)/6 accurately to avoid overflow. */
    seq_t a1 = (seq_t)m;
    seq_t b1 = (seq_t)(m + 1);
    seq_t c1 = (seq_t)(2 * (seq_t)m + 1);

    /* Divide by 2 – one of a1 or b1 is even. */
    if (a1 % 2 == 0)
        a1 /= 2;
    else
        b1 /= 2;

    /* Divide by 3 – exactly one of a1, b1, or c1 is divisible by 3. */
    if (a1 % 3 == 0)
        a1 /= 3;
    else if (b1 % 3 == 0)
        b1 /= 3;
    else
        c1 /= 3;

    seq_t sum = a1 * b1;
    sum = sum * c1;          /* sum now holds S(m) */

    return sum * sum;        /* return the square of the odd square pyramidal number */
}
