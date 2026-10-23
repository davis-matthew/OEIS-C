/*
 * A190093  Number of rhombuses on an (n+1) X 5 grid.
 * https://oeis.org/A190093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + 28*n - 84 if n > 7 and n even a(n) = n^2 + 28*n -
 *            85 if n > 7 and n odd Initial values for n ≤ 7: a(1)=4, a(2)=15,
 *            a(3)=36, a(4)=66, a(5)=96, a(6)=130, a(7)=164
 * Range:     u8 n <= 9, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms for n = 1 .. 7 */
    static const seq_t init[] = {4, 15, 36, 66, 96, 130, 164};
    if (n <= sizeof(init) / sizeof(init[0])) {
        return init[n - 1];
    }

    /* For n > 7 use the closed‑form expressions */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + (seq_t)28 * nn;
    if ((n & 1) == 0) {
        val -= 84;
    } else {
        val -= 85;
    }
    return val;
}
