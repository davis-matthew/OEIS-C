/*
 * A080923  First differences of A003946.
 * https://oeis.org/A080923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(n)=8*3^(n-2) for n>=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* For n >= 2 compute 8·3^(n‑2). */
    idx_t exp = n - 2;          /* non‑negative */
    seq_t result = 1;           /* accumulates 3^exp */
    const seq_t three = (seq_t)3;
    const seq_t eight = (seq_t)8;

    for (idx_t i = 0; i < exp; ++i)
        result *= three;

    result *= eight;
    return result;
}
