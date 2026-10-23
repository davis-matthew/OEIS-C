/*
 * A044999  Numbers whose base-3 representation contains exactly two 0's and
 *          no 2's.
 * https://oeis.org/A044999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest integer whose base‑3 representation
 *            contains exactly two 0’s and no 2’s. The numbers are generated
 *            by enumerating all base‑3 strings of the form 1 followed by
 *            (L‑2) other digits, of which exactly two are 0 and the rest are
 *            1. They are ordered by increasing numeric value,...
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A044999 (n ≥ 1, 1‑based). */
seq_t a(idx_t n)
{
    /* Find the length L of the base‑3 representation of a(n).  Length
       includes the leading 1 and has at least 3 digits. */
    idx_t L = 3;          /* smallest possible length */
    idx_t count = 0;      /* number of terms of smaller lengths */

    for (;;) {
        idx_t len = L - 1;              /* number of positions after the leading digit */
        idx_t comb;                     /* C(len, 2) */

        /* compute comb without overflow */
        if (len & 1U)
            comb = len * ((len - 1U) / 2U);
        else
            comb = (len / 2U) * (len - 1U);

        if (count + comb >= n)
            break;                     /* a(n) lies in this length */

        count += comb;
        ++L;
    }

    /* offset of a(n) within its length (zero‑based). */
    idx_t offset = n - count - 1U;

    /* Find the two 0‑positions (p > q).  Positions are numbered 0..L‑2
       with 0 the least significant digit.  The order is:
         p = L‑2 down to 1, and for each p, q = p‑1 down to 0.  */
    idx_t p = 0U, q = 0U;
    idx_t cum = 0U;                  /* cumulative count of groups visited so far */

    for (p = L - 2U; p > 0U; --p) {
        idx_t groupSize = p;         /* number of qs for this p (q = 0..p‑1) */
        if (cum + groupSize > offset) {
            idx_t within = offset - cum;          /* index inside the current group */
            q = (p - 1U) - within;               /* q is counted from most to least */
            break;
        }
        cum += groupSize;
    }

    /* Compute powers of 3 needed to form the value:
         base   = 3^(L‑1)   (the leading digit)
         sumAll = Σ_{j=0}^{L‑2} 3^j = (base‑1)/2
         pow_p  = 3^p
         pow_q  = 3^q
      Then a(n) = base + sumAll – pow_p – pow_q. */
    seq_t base = 1;
    for (idx_t i = 0U; i < L - 1U; ++i)
        base *= 3;

    seq_t sumAll = (base - 1) / 2;   /* integer division, safe because base is odd */

    seq_t pow_p = 1;
    for (idx_t i = 0U; i < p; ++i)
        pow_p *= 3;

    seq_t pow_q = 1;
    for (idx_t i = 0U; i < q; ++i)
        pow_q *= 3;

    return base + sumAll - pow_p - pow_q;
}
