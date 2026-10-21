/*
 * A207793  Number of (n+1) X 3 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an unequal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A207793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 121*a(n-2) + 356*a(n-3) + 96*a(n-4) -
 *            3604*a(n-5) - 18396*a(n-6) - 28488*a(n-7) + 1440*a(n-8) +
 *            10656*a(n-9) + 3456*a(n-10) Initial values (offset 1):
 *            a(1)=2208, a(2)=37408, a(3)=643968, a(4)=11074704,
 *            a(5)=190505040, a(6)=3276916896, a(7)=56368726448,
 *            a(8)=969636795792, a(9)=16...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n <= 0 as undefined; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Initial 10 terms */
    static const seq_t init[10] = {
        2208ULL,
        37408ULL,
        643968ULL,
        11074704ULL,
        190505040ULL,
        3276916896ULL,
        56368726448ULL,
        969636795792ULL,
        16679397182912ULL,
        286913899633552ULL
    };

    if (n <= 10) return init[n - 1];

    /* Array holding a(i-1) .. a(i-10) for the current step i */
    seq_t cur[10];
    /* For i = 11, cur[0] = a(10), cur[1] = a(9), ..., cur[9] = a(1) */
    for (int k = 0; k < 10; ++k)
        cur[k] = init[9 - k];

    for (idx_t i = 11; i <= n; ++i) {
        /* Compute a(i) using the recurrence with only non‑negative
           intermediate values */
        seq_t next = (seq_t)9   * cur[0] + (seq_t)121 * cur[1]
                   + (seq_t)356 * cur[2] + (seq_t)96  * cur[3]
                   + (seq_t)1440* cur[7] + (seq_t)10656* cur[8]
                   + (seq_t)3456* cur[9];

        /* Subtract the terms with negative coefficients */
        next -= (seq_t)3604  * cur[4];
        next -= (seq_t)18396 * cur[5];
        next -= (seq_t)28488 * cur[6];

        /* Shift the array for the next iteration */
        for (int k = 9; k >= 1; --k)
            cur[k] = cur[k - 1];
        cur[0] = next;
    }

    return cur[0];
}
