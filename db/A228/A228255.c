/*
 * A228255  Number of third differences of arrays of length n+3 of numbers in
 *          0..3.
 * https://oeis.org/A228255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A228255: Number of third differences of arrays of length n+3
 * of numbers in 0..3.  
 *
 * Formula (valid for n > 5):  a(n) = 4^(n+3) - 3^(n+3)
 * Recurrence (valid for n > 7):  a(n) = 7*a(n-1) - 12*a(n-2)
 * Initial values:  a(1)=25, a(2)=319, a(3)=2511, a(4)=13933,
 *                  a(5)=58911, a(6)=242461, a(7)=989527
 */

seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1, undefined for n = 0 */
        return (seq_t)0;

    static const seq_t init[8] = {
        [0] = 0,          /* unused, a(0) */
        [1] = 25,
        [2] = 319,
        [3] = 2511,
        [4] = 13933,
        [5] = 58911,
        [6] = 242461,
        [7] = 989527
    };

    if (n <= 7)
        return init[n];

    /* Use recurrence a(n) = 7*a(n-1) - 12*a(n-2) for n >= 8 */
    seq_t a_nm2 = init[6];   /* a(n-2) for first iteration (n=8) */
    seq_t a_nm1 = init[7];   /* a(n-1) for first iteration (n=8) */
    seq_t cur   = 0;

    for (idx_t i = 8; i <= n; ++i) {
        cur = 7 * a_nm1 - 12 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
