/*
 * A231404  Integers n dividing the Lucas sequence u(n), where u(i) = 2*u(i-1)
 *          - 4*u(i-2) with initial conditions u(0)=0, u(1)=1.
 * https://oeis.org/A231404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth element of the sorted union of powers of 2 and
 *            positive multiples of 3 (the set {2^k : k≥0} ∪ {3m : m≥1})
 * Range:     u8 n <= 50, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; for n < 1 the behaviour is undefined */
    seq_t curPow = 1;          /* current 2^k  (k starts at 0)      */
    seq_t curMul = 3;          /* current 3*m  (m starts at 1)       */
    idx_t  count = 0;          /* how many items produced so far    */
    seq_t  result = 0;

    while (count < n) {
        if (curPow < curMul) {
            result = curPow;
            curPow <<= 1; /* advance to next power of 2 */
        } else {
            result = curMul;
            curMul += (seq_t)3; /* advance to next multiple of 3 */
        }
        ++count;
    }
    return result;
}
