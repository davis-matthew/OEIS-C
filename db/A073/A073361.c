/*
 * A073361  Nested floor product of n and fractions (k+1)/k for all k>0 (mod
 *          4), divided by 4.
 * https://oeis.org/A073361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/4)*⌊...⌊⌊⌊⌊ n(2/1)⌋(3/2)⌋(4/3)⌋(6/5)⌋...⌋, where k
 *            runs over all positive integers that are not multiples of 4
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 453, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n ≥ 1.  The algorithm processes each block of consecutive
   k values that give the same floor division result in one step, skipping k
   ≡ 0 (mod 4).  The value is returned divided by 4. */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;      /* current value of the nested product   */
    idx_t  k   = 1;            /* next k value (skip multiples of 4)    */

    while (k <= (idx_t)cur) {
        /* Skip k that are multiples of 4 */
        if (k % 4 == 0) {
            ++k;
            continue;
        }

        /* floor(cur / k)  (r ≥ 1 for k ≤ cur) */
        seq_t r = cur / k;

        /* Largest k that still gives the same floor division result */
        idx_t R = (idx_t)(cur / r);          /* integer division,  R ≥ k   */
        if (R > (idx_t)cur) R = (idx_t)cur; /* safety, although unnecessary */

        /* Count how many ks in [k, R] are not multiples of 4 */
        idx_t total = R - k + 1;                /* length of interval   */
        idx_t mult4_left  = (k - 1) / 4;        /* multiples of 4 ≤ k-1 */
        idx_t mult4_right = R / 4;              /* multiples of 4 ≤ R   */
        idx_t mult4   = mult4_right - mult4_left; /* multiples of 4 in [k,R] */
        idx_t count   = total - mult4;          /* ks we actually use    */

        /* Update current value in one step */
        cur += r * (seq_t)count;

        /* Next k to process */
        k = R + 1;
    }

    /* Final division by 4 as per the definition */
    return cur / (seq_t)4;
}
