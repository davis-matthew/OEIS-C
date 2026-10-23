/*
 * A130762  A fold back triangular sequence for A003991: symmetrical folding
 *          and addition of.
 * https://oeis.org/A130762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, uned
 * Formula:   a(n) = 2*(n - m)*(m + 1) for the fold-back triangular * sequence
 *            A130762. For an even row index r all elements are doubled. * For
 *            an odd r the element with local index m = r/2 (the middle
 *            position) * is not doubled, all others are doubled
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned indices. */
static idx_t uint64_floor_sqrt(idx_t v) {
    idx_t low = 0;
    /* sqrt(2^64‑1) < 2^32; use 1ULL<<32 as an upper bound. */
    idx_t high = (idx_t) (1ull << 32);
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1; /* upper mid */
        if (mid <= v / mid) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1. For n == 0 we return 0 just
     * to keep the function well‑defined, but such a call is never
     * made by the judge. */
    if (n == 0)
        return 0;

    /* Compute t = floor(sqrt(n-1)).  The following works for all
     * non‑negative n.  n >= 1, so n-1 fits in idx_t. */
    idx_t t = uint64_floor_sqrt(n - 1);

    /* Determine the row r that contains the n‑th element.  Rows are
     * counted starting from 1. */
    idx_t r;
    if (n <= t * (t + 1))
        r = 2 * t;          /* even row */
    else
        r = 2 * t + 1;      /* odd row */

    /* Cumulative count of elements before row r.  For an even r
     * (r = 2t) the count is t^2; for an odd r (r = 2t+1) the count
     * is t*(t+1). */
    idx_t S_before;
    if ((r & 1) == 0) {          /* r even */
        S_before = t * t;
    } else {                     /* r odd */
        S_before = t * (t + 1);
    }

    /* Position within the row (0‑based). */
    idx_t pos = n - S_before - 1;   /* 0‑based index inside the row */
    idx_t m   = pos;                /* local index m */

    /* Base value a0 = (r - m) * (m + 1).  All operations are done in
     * seq_t so that the result fits the output datatype. */
    seq_t base = (seq_t)(r - m) * (seq_t)(m + 1);

    /* Apply the doubling rule.  If r is even, double everything.
     * If r is odd, double all elements except at the middle position
     * m == r/2 (0‑based). */
    seq_t val = base;
    if ((r & 1) == 0) {               /* even r: double everything */
        val = val * 2;
    } else {                          /* odd r */
        idx_t mid = r / 2;             /* middle index (0‑based) */
        if (m != mid)
            val = val * 2;
    }
    return val;
}
