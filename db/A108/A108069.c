/*
 * A108069  Number of ordered pairs (i,j) with i>=1, j>=1 and i!*j! = n.
 * https://oeis.org/A108069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ (i,j) ∈ N^2, i≥1, j≥1 : i! * j! = n }|. This function
 *            computes the number of ordered pairs of factorials whose product
 *            equals the given n. The result is always ≤ 4, as conjectured for
 *            this sequence
 * Range:     u8 n <= 1873, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to the seq_t type for factorial arithmetic */
    seq_t target = (seq_t)n;

    /* store factorials up to the point where i! > n.  25 is safe for any
       n that fits in uint64_t (since 20! < 2^64). */
    seq_t facts[25];
    idx_t fact_count = 0;

    /* compute factorials 1!, 2!, ... until i! > target */
    seq_t fact = 1;
    idx_t i = 1;
    while (1) {
        fact *= (seq_t)i;          /* i! */
        if (fact > target)
            break;
        facts[fact_count++] = fact;
        ++i;
    }

    /* count ordered pairs (i,j) such that i! * j! = n */
    seq_t count = 0;
    for (idx_t p = 0; p < fact_count; ++p) {
        seq_t fi = facts[p];
        if (fi == 0)        /* should never happen */
            continue;
        if (target % fi != 0)
            continue;

        seq_t q = target / fi;  /* candidate j! */
        /* check if q is one of the computed factorials (exists j) */
        for (idx_t qidx = 0; qidx < fact_count; ++qidx) {
            if (facts[qidx] == q) {
                ++count;      /* ordered pair (i,j) found */
                break;
            }
        }
    }

    return count;
}
