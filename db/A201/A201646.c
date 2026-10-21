/*
 * A201646  The second of the five known sets of nine distinct odd numbers the
 *          sum of whose reciprocals is 1.
 * https://oeis.org/A201646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth term of the second known set of nine distinct
 *            odd numbers whose reciprocal sum is 1
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t arr[9] = {
        (seq_t)3,   (seq_t)5,   (seq_t)7,
        (seq_t)9,   (seq_t)11,  (seq_t)15,
        (seq_t)21,  (seq_t)135, (seq_t)10395
    };
    if (n < 1 || n > 9) return (seq_t)0;
    return arr[n - 1];
}
