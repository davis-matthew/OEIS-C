/*
 * A026471  a(n) = least positive integer > a(n-1) and not of the form a(i) +
 *          a(j) + a(k) for 1 <= i < j < k <= n.
 * https://oeis.org/A026471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=13 For n≥7: let q =
 *            floor((n-7)/5) r = (n-7)%5 a(n) = 13 + 23*q + Δ[r] where Δ =
 *            {1,2,12,13,14} corresponding to residues 14,15,2,3,4 (mod 23)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first six terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)5;
    if (n == 6) return (seq_t)13;

    /* for n ≥ 7, apply the closed‑form formula */
    static const seq_t delta[] = { (seq_t)1, (seq_t)2,
                                   (seq_t)12, (seq_t)13,
                                   (seq_t)14 };
    idx_t k = n - (idx_t)7;          /* k ≥ 0 */
    idx_t q = k / (idx_t)5;          /* integer division */
    idx_t r = k % (idx_t)5;          /* 0 ≤ r < 5 */

    seq_t result = (seq_t)13;
    result += (seq_t)q * (seq_t)23;
    result += delta[r];
    return result;
}
