/*
 * A128207  Number triangle
 *          T(n,k)=(-1)^(n-k)*[k<=n]*Product{i=k+1..n,Sum{j=0..i-1,F(j-1)}}.
 * https://oeis.org/A128207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = (-1)^(r- c ) ⋅ ∏_{i = c+1…r} F(i) where r = ⌊(√(8·n + 1)
 *            – 1)/2⌋ (row index in the triangle), c = n – r(r+1)/2 (column
 *            index), and F(i) is the i‑th Fibonacci number (F(0)=0, F(1)=1,
 *            F(2)=1, …, with the convention F(−1)=1 so that the sums in the
 *            definition reduce to F(i))
 * Range:     i8 n <= 20, i16 n <= 35, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static inline idx_t triangle_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* First find an upper bound for r. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search between low and high for the correct r. */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* r is the largest index satisfying the inequality */
}

seq_t a(idx_t n)
{
    idx_t r = triangle_row(n);                     /* row index   */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2); /* first index of row r */
    idx_t c = n - start;                            /* column index */

    /* Compute the product ∏_{i = c+1…r} F(i) on the fly,
       using only two Fibonacci values at a time.   */
    seq_t prod = (seq_t)1;
    seq_t fprev = (seq_t)0;     /* F(0) */
    seq_t fcur  = (seq_t)1;     /* F(1) */

    for (idx_t i = 1; i <= r; ++i)
    {
        /* i represents F(i).  Multiply only if i is within [c+1, r]. */
        if (i > c)
            prod *= fcur;

        /* Advance to next Fibonacci value. */
        seq_t fnext = fcur + fprev;
        fprev = fcur;
        fcur  = fnext;
    }

    /* Apply the sign (-1)^(r-c).  */
    if (((r - c) & 1) != 0)
        prod = -prod;

    return prod;
}
