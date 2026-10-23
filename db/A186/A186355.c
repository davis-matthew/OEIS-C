/*
 * A186355  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=3i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186354.
 * https://oeis.org/A186355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + sum_{k=1}^{n} floor((k+1)/3)
 * Range:     u8 n <= 35, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Closed‑form implementation: let p = n / 3, s = n % 3
   If s == 0 : a = (3p^2 + 7p) / 2
   If s == 1 : a = (3p^2 + 9p + 2) / 2
   If s == 2 : a = (3p^2 + 11p + 6) / 2  */
seq_t a(idx_t n)
{
    idx_t p = n / 3;
    idx_t s = n % 3;
    seq_t res;
    if (s == 0) {
        res = ((seq_t)3 * p * p + (seq_t)7 * p) / 2;
    } else if (s == 1) {
        res = ((seq_t)3 * p * p + (seq_t)9 * p + 2) / 2;
    } else {
        res = ((seq_t)3 * p * p + (seq_t)11 * p + 6) / 2;
    }
    return res;
}
