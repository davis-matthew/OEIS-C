/*
 * A131340  Number of 0's in minimal Lucas representation (A130310) of n.
 * https://oeis.org/A131340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0's in the minimal Lucas * representation of n
 *            (offset 1). The minimal representation * is obtained by a greedy
 *            subtraction of Lucas numbers, * always choosing the largest
 *            remaining Lucas number that * does not exceed the current
 *            remainder. Because in the * Lucas sequence the fir...
 * Range:     u8 n <= 376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset is 1; input n ≥ 1 */

    /* Build an array of Lucas numbers up to n   */
    /* Lucas numbers: L0=2, L1=1, L2=3, L3=4, …   */
    /* We keep the natural index ordering.       */
    const int MAX_LUCAS = 65;  /* enough for n up to 10 000 */
    seq_t lucas[MAX_LUCAS];
    /* initial values */
    lucas[0] = (seq_t)2;
    lucas[1] = (seq_t)1;
    int cnt = 2;
    while (cnt < MAX_LUCAS) {
        seq_t next = lucas[cnt - 1] + lucas[cnt - 2];
        if (next > (seq_t)n)
            break;
        lucas[cnt++] = next;
    }

    /* Greedy subtraction to build the minimal representation */
    seq_t rem = (seq_t)n;
    int ones = 0;
    int max_used = -1;

    for (int i = cnt - 1; i >= 0 && rem > 0; --i) {
        seq_t val = lucas[i];
        if (val <= rem) {
            /* Special case: the first two terms are out of
             * order (2,1).  If the remainder is 2 or more,
             * we must skip the '1' before using the '2'.
             */
            if (i == 1 && rem >= 2)
                continue;

            rem -= val;
            ++ones;
            if (i > max_used)
                max_used = i;
        }
    }

    /* Number of 0's = total bits (max_used+1) minus number of 1's */
    seq_t zeros = (seq_t)(max_used + 1) - (seq_t)ones;
    return zeros;
}
