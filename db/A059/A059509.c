/*
 * A059509  Main diagonal of the array A059503.
 * https://oeis.org/A059509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4) initial values:
 *            a(1)=1, a(2)=5, a(3)=19, a(4)=66
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* value below offset is irrelevant */
        return 0;
    }
    /* handle the first four terms explicitly */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)5;
        case 3: return (seq_t)19;
        case 4: return (seq_t)66;
    }

    /* slide a recurrence from 5 up to n */
    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)5;   /* a(n-3) */
    seq_t a3 = (seq_t)19;  /* a(n-2) */
    seq_t a4 = (seq_t)66;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a5 = (seq_t)6 * a4 - (seq_t)11 * a3 + (seq_t)6 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }
    return a4;
}
