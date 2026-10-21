/*
 * A088370  Triangle T(n,k), read by rows, where the n-th row is a binary
 *          arrangement of the numbers 1 through n.
 * https://oeis.org/A088370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9881, u16 n <= 9881, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the nth element read row‑by‑row from triangle T.
   T(1) = {1}.
   For r > 1:
     if r is even:  T(r) = concat( 2*x-1 | x∈T(r/2),   2*x | x∈T(r/2) )
     if r is odd :  T(r) = concat( 2*x-1 | x∈T((r+1)/2), 2*x | x∈T((r-1)/2) )
   Initial values: a(1)=1, a(2)=1, a(3)=2, a(4)=1, a(5)=3, a(6)=2, … */

static seq_t
get_val(idx_t r, idx_t p)
{
    if (r <= 1)            /* base row */
        return (seq_t)1;

    if ((r & 1) == 0)          /* r even : rows of length r/2 twice */
    {
        idx_t k = r >> 1;      /* r/2 */
        if (p <= k)
            return (seq_t)2 * get_val(k, p) - (seq_t)1;
        else
            return (seq_t)2 * get_val(k, p - k);
    }
    else                       /* r odd : first part from (r+1)/2, second from (r-1)/2 */
    {
        idx_t k = (r + 1) >> 1;        /* ceil(r/2) = (r+1)/2 */
        if (p <= k)
            return (seq_t)2 * get_val(k, p) - (seq_t)1;
        else
            return (seq_t)2 * get_val(r - k, p - k);  /* r-k = (r-1)/2 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* not used, but keeps function total */

    /* Find the row r satisfying T(r-1) < n <= T(r), where T(r)=r*(r+1)/2. */
    idx_t r = 1;
    idx_t cum = 1;                 /* cumulative number of terms up to row r */

    while (cum < n) {
        ++r;
        cum += r;                  /* cum = T(r) */
    }

    idx_t prev = cum - r;          /* T(r-1) = cum - r */
    idx_t p = n - prev;            /* position inside the row (1‑based) */

    return get_val(r, p);
}
