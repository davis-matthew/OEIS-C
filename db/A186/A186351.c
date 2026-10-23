/*
 * A186351  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the odd numbers and the
 *          triangular numbers. Complement of A186350.
 * https://oeis.org/A186351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2. For n ≥ 2: a(n) = n + 1 + Σ_{k=2}^{n} ceil(k/2) – (#
 *            of k in [2,n] with k ≡ 3 (mod 4)). Let m = ⌊n/2⌋. Then: if n is
 *            even: a(n) = m*(m+1) + 2*m – ⌊(2m–3)/4⌋ – 1 if n is odd: a(n) =
 *            (m+1)^2 + 4*m + 1 – ⌊(2m–2)/4⌋
 * Range:     u8 n <= 29, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    seq_t n_seq = (seq_t)n;
    seq_t m      = n_seq / 2;          /* floor(n/2) */

    seq_t sum_c1;
    if (n & 1) {                       /* n odd */
        sum_c1 = (m + 1) * (m + 1) - 1;
    } else {                           /* n even */
        sum_c1 = m * (m + 1) - 1;
    }

    seq_t count3 = 0;                  /* number of k∈[2,n] with k%4==3 */
    if (n >= 3) {
        count3 = ((n - 3) / 4) + 1;
    }

    return n_seq + 1 + sum_c1 - count3;
}
