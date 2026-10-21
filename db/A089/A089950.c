/*
 * A089950  Partial sums of A001652.
 * https://oeis.org/A089950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000129(n+1)^2 - floor((n+2)/2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000129 are Pell numbers: P(0)=0, P(1)=1,
   P(k)=2*P(k-1)+P(k-2) for k ≥ 2. */

seq_t a(idx_t n)
{
    /* Compute the (n+1)-th Pell number (P_{n+1}). */
    seq_t p0 = (seq_t)0;  /* P(0) */
    seq_t p1 = (seq_t)1;  /* P(1) */
    idx_t i;
    idx_t target = n + 1;

    /* For target = 0 or 1 we already have p1 = P_{target} */
    for (i = 2; i <= target; ++i) {
        seq_t p = p1 + p1 + p0;   /* 2*P_{i-1} + P_{i-2} */
        p0 = p1;
        p1 = p;
    }

    /* Result: P_{n+1}^2 - floor((n+2)/2) */
    seq_t result = p1 * p1 - (seq_t)((n + 2) / 2);
    return result;
}
