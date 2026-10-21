/*
 * A229692  Number of defective 3-colorings of an n X 6 0..2 array connected
 *          horizontally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 75*a(n-1) - 2145*a(n-2) + 29368*a(n-3) - 205200*a(n-4) *
 *            + 803115*a(n-5) - 1842183*a(n-6) + 2460375*a(n-7) * -
 *            1771470*a(n-8) + 531441*a(n-9) for n > 11 * Initial values: *
 *            a(1)=40, a(2)=5077, a(3)=230383, a(4)=8186713, a(5)=258006388, *
 *            a(6)=7551515197, a(7)=210095323918, a(8)=56321226258...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

static const seq_t init_vals[11] = {
    (seq_t)40ULL,
    (seq_t)5077ULL,
    (seq_t)230383ULL,
    (seq_t)8186713ULL,
    (seq_t)258006388ULL,
    (seq_t)7551515197ULL,
    (seq_t)210095323918ULL,
    (seq_t)5632122625852ULL,
    (seq_t)146747625919741ULL,
    (seq_t)3738362679893995ULL,
    (seq_t)93505337317628617ULL
};

static const int coeffs[9] = {
    75,     -2145,   29368,   -205200,  803115,
   -1842183, 2460375, -1771470, 531441
};

seq_t a(idx_t n)
{
    /* Handle the range from the start */
    if (n == 0) return (seq_t)0;
    if (n <= 11) return init_vals[n - 1];

    /* Buffer holding the last 9 computed terms
       buffer[0] = a(i-1), buffer[1] = a(i-2), … , buffer[8] = a(i-9)   */
    seq_t buffer[9];

    /* Initialize buffer for i = 12 (so buffer holds a11 … a3) */
    buffer[0] = init_vals[10]; /* a11 */
    buffer[1] = init_vals[9];  /* a10 */
    buffer[2] = init_vals[8];  /* a9  */
    buffer[3] = init_vals[7];  /* a8  */
    buffer[4] = init_vals[6];  /* a7  */
    buffer[5] = init_vals[5];  /* a6  */
    buffer[6] = init_vals[4];  /* a5  */
    buffer[7] = init_vals[3];  /* a4  */
    buffer[8] = init_vals[2];  /* a3  */

    seq_t current = (seq_t)0;  /* will hold a(i) */

    for (idx_t i = 12; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 0; k < 9; ++k) {
            seq_t prod = seq_mul(buffer[k], (seq_t)(k < 0 ? 0 : (coeffs[k] > 0 ? coeffs[k] : -coeffs[k])));
            if (coeffs[k] > 0) {
                sum = sum + prod;
            } else {
                sum = sum - prod;
            }
        }
        current = sum;

        /* Shift buffer left and insert the new value at position 0 */
        for (int j = 8; j > 0; --j) {
            buffer[j] = buffer[j - 1];
        }
        buffer[0] = current;
    }

    return current;
}
