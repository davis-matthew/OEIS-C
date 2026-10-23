/*
 * A108068  Number of ordered pairs (i,j) with i>=0, j>=0 and i!*j! = n.
 * https://oeis.org/A108068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{(i,j) | i! * j! = n} (ordered pairs)
 * Range:     u8 n <= 1451, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine enumerates all factorials ≤ n and counts
   pairs whose product is exactly n.  The resulting
   count fits easily in seq_t for any n that fits in idx_t. */

seq_t a(idx_t n)
{
    /* Handle the empty case directly */
    if (n == 0) {
        return 0;
    }

    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* Store factorials up to the first one exceeding N. */
    /* 25 is safe for all n that fit in idx_t and for 128‑bit seq_t. */
    seq_t facts[25];
    idx_t k = 0;
    seq_t f = 1;
    facts[k++] = f;               /* 0! = 1 */

    /* Compute successive factorials until the next one would exceed N. */
    for (idx_t i = 1; /* nothing */; i++)
    {
        f = f * (seq_t)i;        /* f = i! */
        if (f > N) {
            break;
        }
        facts[k++] = f;
    }

    /* Count ordered pairs (i,j).   Since the list of factorials is very short
       (≤ about 20 entries for 64‑bit limits), a simple double loop is very
       efficient.  We avoid multiplication overflow by using division. */
    seq_t count = 0;
    for (idx_t a = 0; a < k; ++a) {
        for (idx_t b = 0; b < k; ++b) {
            if (N % facts[a] == 0 && N / facts[a] == facts[b]) {
                ++count;
            }
        }
    }

    return count;
}
