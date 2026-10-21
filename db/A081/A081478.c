/*
 * A081478  Consider the mapping f(a/b) = (a - b)/(ab). Taking a = 2 and b = 1
 *          to start with and carrying out this mapping repeatedly on each new
 *          (reduced) rational number gives the following sequence
 *          2/1,1/2,-1/2,-3/-2,-1/6,... Sequence contains the denominators.
 * https://oeis.org/A081478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 10, i32 n <= 12, i64 n <= 14, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081478: denominators of the fractions obtained by iterating
   f(a/b) = (a - b)/(a*b) starting from 2/1.
   Let k = (n+1)/2 and let c₁ = 1, c_{i+1} = c_i*(c_i+1).
   Then a(2k-1) = c_k, and for k ≥ 2, a(2k) = -c_k.
   Base cases: a(1) = 1, a(2) = 2. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    idx_t k = (n + 1) / 2;      /* index in the odd‑term subsequence */
    seq_t c = (seq_t)1;         /* c₁ */

    for (idx_t i = 1; i < k; ++i)
        c = c * (c + (seq_t)1);

    return (n & 1) ? c : -c;
}
