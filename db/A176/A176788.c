/*
 * A176788  a(n) = (0!! + 1!! + 2!! + 3!! + ... + (n-1)!!) mod n.
 * https://oeis.org/A176788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (0!! + 1!! + 2!! + ... + (n-1)!!) mod n
 * Range:     u8 n <= 217, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but keeps type stable */

    seq_t sum = 0;
    seq_t even = 1;  /* 0!! */
    seq_t odd  = 1;  /* 1!! */

    for (idx_t i = 0; i < n; ++i) {
        seq_t val;

        if (i == 0) {
            val = even;   /* 0!! = 1 */
        } else if (i == 1) {
            val = odd;    /* 1!! = 1 */
        } else if ((i & 1) == 0) {        /* even i */
            val = ((seq_t)i * even) % (seq_t)n;
            even = val;
        } else {                          /* odd i */
            val = ((seq_t)i * odd) % (seq_t)n;
            odd = val;
        }

        sum += val;
        sum %= (seq_t)n;
    }

    return sum;
}
