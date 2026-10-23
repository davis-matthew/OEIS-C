/*
 * A169411  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169411.
   Generating function:
      G(t) = (t^32 + 2*t^31 + … + 2*t + 1) / (78*t^32 - 12*t + 1)
   For n ≥ 32 the linear recurrence is
      a(n) = 12·(a(n-1)+…+a(n-31)) – 78·a(n-32)
   The first 32 terms are obtained from the power‑series expansion:
      a(0)=1,
      a(i)=2 + 12·(a(i-1)+…+a(0))   for 1 ≤ i ≤ 31          */
/* Note: the function is O(n) and uses only 32 sequence elements;
   no dynamic allocation and no external utilities are required. */

seq_t a(idx_t n)
{
    /* Special case for the very first term */
    if (n == 0)
        return (seq_t)1;

    /* Compute the first min(n,31)+1 terms (0..min(n,31)) */
    seq_t buf[32];
    buf[0] = (seq_t)1;                     /* a(0) */

    idx_t maxInitial = n < 31 ? n : 31;    /* last index to compute initially */
    for (idx_t i = 1; i <= maxInitial; ++i) {
        seq_t sumPrev = 0;
        for (idx_t j = 1; j <= i; ++j)
            sumPrev += buf[i - j];          /* sum of a(i-1)…a(0) */
        buf[i] = (seq_t)2 + (seq_t)12 * sumPrev; /* a(i) = 2 + 12·sumPrev */
    }

    /* If the requested index is within the first 32 terms, return it. */
    if (n <= 31)
        return buf[n];

    /* Compute terms from 32 up to n using the recurrence. */
    for (idx_t k = 32; k <= n; ++k) {
        /* sumPrev = a(k-1)+…+a(k-31) i.e. buf[1]…buf[31] */
        seq_t sumPrev = 0;
        for (idx_t j = 1; j <= 31; ++j)
            sumPrev += buf[j];

        /* a(k) = 12·sumPrev – 78·a(k-32) where a(k-32)=buf[0] */
        seq_t newVal = (seq_t)12 * sumPrev - (seq_t)78 * buf[0];

        /* Shift the circular buffer left (drop oldest, add newest) */
        for (idx_t j = 0; j < 31; ++j)
            buf[j] = buf[j + 1];
        buf[31] = newVal;
    }

    /* After the loop, buf[31] holds a(n) */
    return buf[31];
}
