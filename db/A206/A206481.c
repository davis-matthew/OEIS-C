/*
 * A206481  a(n) + a(n+2) = n^3.
 * https://oeis.org/A206481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206481: a(n)+a(n+2)=n³ (offset 1)  
 * Recurrence: a(n) = (n-2)³ - a(n-2)  
 * Initial values: a(1)=0, a(2)=1
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    if ((n & 1ULL) == 0) {            /* even n */
        seq_t prev = (seq_t)1;        /* a(2) */
        for (idx_t i = 4; i <= n; i += 2) {
            seq_t k   = (seq_t)(i - 2);
            seq_t cube = k * k * k;   /* (i-2)³ */
            prev = cube - prev;       /* a(i) = (i-2)³ - a(i-2) */
        }
        return prev;
    } else {                          /* odd n */
        seq_t prev = (seq_t)0;        /* a(1) */
        for (idx_t i = 3; i <= n; i += 2) {
            seq_t k   = (seq_t)(i - 2);
            seq_t cube = k * k * k;   /* (i-2)³ */
            prev = cube - prev;       /* a(i) = (i-2)³ - a(i-2) */
        }
        return prev;
    }
}
