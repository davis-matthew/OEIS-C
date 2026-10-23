/*
 * A038620  Growth function (or coordination sequence) of the infinite cubic
 *          graph corresponding to the srs net (a(n) = number of nodes at
 *          distance n from a fixed node).
 * https://oeis.org/A038620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=6; for n>=3: if n % 3 == 0: a(n) = 4*n^2/3;
 *            if n % 3 == 1: a(n) = (4*n^2 + n + 4)/3; if n % 3 == 2: a(n) =
 *            (4*n^2 - n + 10)/3
 * Range:     u8 n <= 7, u16 n <= 131, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch ((uint64_t)n) {
        case 0ULL: return (seq_t)1;
        case 1ULL: return (seq_t)3;
        case 2ULL: return (seq_t)6;
    }

    /* n >= 3 */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;      /* n^2  */
    seq_t base = 4 * n2;       /* 4*n^2 */

    switch (n % 3ULL) {
        case 0ULL:
            return base / (seq_t)3;
        case 1ULL:
            return (base + nn + (seq_t)4) / (seq_t)3;
        default: /* n % 3 == 2 */
            return (base - nn + (seq_t)10) / (seq_t)3;
    }
}
