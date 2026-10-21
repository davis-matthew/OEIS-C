/*
 * A054112  T(n,n-3), array T as in A054110.
 * https://oeis.org/A054112
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 - 7*n)/3 for n > 6; a(3) = 1, a(4) = 10, a(5) = 28,
 *            a(6) = 57
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values below the offset are not used. */
    if (n < 3) return 0;

    /* Handle the special cases for n = 3..6. */
    switch (n)
    {
        case 3: return (seq_t)1;
        case 4: return (seq_t)10;
        case 5: return (seq_t)28;
        case 6: return (seq_t)57;
        default: break; /* n >= 7 */
    }

    /* Compute (n^3 - 7*n)/3 using seq_t arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t val = (nn * nn * nn - (seq_t)7 * nn) / (seq_t)3;
    return val;
}
