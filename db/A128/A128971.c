/*
 * A128971  A130125 * A000012.
 * https://oeis.org/A128971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the triangle defined by t(r,j) = a(r)
 *            (r even, j==0) = a(r) – a(2*((j+1)/2)–2) (r even, j>0) = a(r) –
 *            a(2*(j/2)–1) (r odd , j>=0) where a(0)=1 and a(k)=2·a(k-1)+[k
 *            even]
 * Range:     u8 n <= 34, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use only type idx_t (unsigned 64‑bit) and seq_t (unsigned integer
   of the chosen precision).  No other types are required. */
#include <stdint.h>          /* for __int128, not required but harmless */

/* Compute the auxiliary sequence a(k) = 2*a(k-1)+[k even]. */
static seq_t compute_a(idx_t k)
{
    seq_t v = 1;            /* a(0) = 1 */
    for (idx_t i = 1; i <= k; ++i) {
        v <<= 1;           /* v = 2*v */
        if ((i & 1) == 0)  /* add 1 when i is even */
            v += 1;
    }
    return v;
}

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t row_of(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);

    /* offset = r*(r+1)/2  (index where row r starts) */
    unsigned __int128 off128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)off128;

    idx_t j = n - offset;            /* column in the row */

    seq_t row_val = compute_a(r);

    if ((r & 1) == 0) {             /* even row */
        if (j == 0)                 /* first element of the row */
            return row_val;

        idx_t g = (j + 1) >> 1;      /* group number (starting from 1) */
        idx_t sub_idx = 2 * (g - 1); /* index for the subtraction */
        seq_t sub = compute_a(sub_idx);
        return row_val - sub;
    } else {                         /* odd row */
        idx_t g = j >> 1;            /* group number (starting from 0) */
        if (g == 0)                  /* first group has no subtraction */
            return row_val;

        idx_t sub_idx = 2 * g - 1;    /* index for the subtraction */
        seq_t sub = compute_a(sub_idx);
        return row_val - sub;
    }
}
