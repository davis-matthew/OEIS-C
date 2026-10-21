/*
 * A220691  Table A(i,j) read by antidiagonals in order A(1,1), A(1,2),
 *          A(2,1), A(1,3), A(2,2), A(3,1), ..., where A(i,j) is the number of
 *          ways in which we can add 2 distinct integers from the range 1..i
 *          in such a way that the sum is divisible by j.
 * https://oeis.org/A220691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A220691(n) is the antidiagonal reading of the table A(i,j),
 *            where A(i,j) = number of ways to choose two distinct integers
 *            from 1..i whose sum is divisible by j. Here n is the position in
 *            the antidiagonal sequence (offset 1)
 * Range:     u8 n <= 299, u16 n <= 9059, u32 n <= 9059, u64 n <= 9059,
 *            u128 n <= 9059, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_J 200   /* sufficient for n ≤ 10000 */

seq_t a(idx_t n)
{
    if (n == 0)       /* offset lower bound (ignored) */
        return (seq_t)0;

    /* locate the antidiagonal: find smallest d with d*(d+1)/2 ≥ n */
    idx_t d = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++d;
        cum += d;
    }

    idx_t prev_cum = (d - 1) * d / 2;
    idx_t offset   = n - prev_cum - 1;   /* position within this diagonal */

    idx_t i = offset + 1;      /* row index  (1 .. d)   */
    idx_t j = d - offset;      /* column index (1 .. d) */

    /* count numbers in 1..i for each residue modulo j */
    seq_t cnt[MAX_J];
    for (idx_t k = 0; k < MAX_J; ++k)
        cnt[k] = (seq_t)0;

    cnt[0] = (seq_t)(i / j);
    for (idx_t r = 1; r < j; ++r) {
        if (r <= i)
            cnt[r] = (seq_t)((i - r) / j + 1);
        else
            cnt[r] = (seq_t)0;
    }

    seq_t total = (seq_t)0;

    /* pairs of numbers both congruent to 0 modulo j */
    if (cnt[0] >= 2)
        total += cnt[0] * (cnt[0] - 1) / 2;

    /* pairs of numbers from complementary residues r and j-r */
    for (idx_t r = 1; r < j; ++r) {
        idx_t s = (j - r) % j;
        if (r < s)                   /* distinct residues */
            total += cnt[r] * cnt[s];
        else if (r == s && cnt[r] >= 2)  /* r == j/2 when j even */
            total += cnt[r] * (cnt[r] - 1) / 2;
    }

    return total;
}
