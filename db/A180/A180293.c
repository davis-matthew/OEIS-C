/*
 * A180293  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-3.
 * https://oeis.org/A180293
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*C(n+1,3) for n>6 Special cases: a(4)=1, a(5)=50,
 *            a(6)=195 (a(n) is defined only for n ≥ 4; smaller n return 0.)
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom3_from_N(idx_t N) {
    /* Compute C(N,3) without overflow for arbitrary precision seq_t.
       Compute as product (N-2)*(N-1)*N / 6 via successive
       multiplications and divisions that keep intermediate results integer.
       This works for all N ≥ 3. */
    seq_t result = (seq_t)1;
    result = (result * (N - 2)) / 1;  /* /1 is a no-op, kept for clarity */
    result = (result * (N - 1)) / 2;
    result = (result * N)        / 3;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)          return 0;      /* values below the offset are undefined */
    if (n == 4)         return 1;      /* explicit values for the offset range */
    if (n == 5)         return 50;
    if (n == 6)         return 195;

    /* For n≥7 use the closed‑form formula */
    seq_t n_t = (seq_t)n;
    seq_t comb = binom3_from_N(n + 1);      /* C(n+1,3) */
    return n_t * comb;
}
