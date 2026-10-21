/*
 * A214613  Abelian complexity function of ordinary paperfolding word
 *          (A014707).
 * https://oeis.org/A214613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Albertic recurrence from OEIS A214613 * a(1) = 2, * a(4n) =
 *            a(2n), * a(4n+2) = a(2n+1)+1, * a(16n+1) = a(8n+1), *
 *            a(16n+{3,7,9,13}) = a(2n+1)+2, * a(16n+5) = a(4n+1)+2
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    /* even n */
    if ((n & 1ULL) == 0) {
        if ((n & 3ULL) == 0)            /* n % 4 == 0 */
            return a(n >> 1);
        /* n % 4 == 2 */
        return a(n >> 1) + (seq_t)1;
    }

    /* n is odd */
    uint64_t r4 = n & 3ULL;          /* n % 4 */
    if (r4 == 3)                     /* n % 4 == 3 */
        return a((n >> 1) + 1) + (seq_t)1;

    /* n % 4 == 1  */
    uint64_t r16 = n & 15ULL;        /* n % 16 */
    switch (r16) {
    case 1:                          /* 16k+1 */
        return a(((n - 1) >> 1) + 1);
    case 5:                          /* 16k+5 */
        return a(((n - 5) >> 2) + 1) + (seq_t)2;
    case 9:
    case 13:                         /* 16k+9 or 16k+13 */
        return a(((n >> 4) << 1) + 1) + (seq_t)2;
    default:                         /* should not occur */
        return a((n >> 1) + 1) + (seq_t)1;
    }
}
