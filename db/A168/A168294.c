/*
 * A168294  Carryless product n times n+1.
 * https://oeis.org/A168294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = carryless product of n and n+1 in base 10
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Extract decimal digits of n and n+1 (least‑significant first) */
    int d[20], e[20];
    int nd = 0, ne = 0;

    idx_t t = n;
    if (t == 0) {
        d[0] = 0;
        nd = 1;
    } else {
        while (t > 0) {
            d[nd++] = (int)(t % 10);
            t /= 10;
        }
    }

    t = n + 1;
    if (t == 0) {
        e[0] = 0;
        ne = 1;
    } else {
        while (t > 0) {
            e[ne++] = (int)(t % 10);
            t /= 10;
        }
    }

    /* Convolution of the digit arrays */
    int g[40];
    for (int i = 0; i < 40; ++i) g[i] = 0;

    for (int i = 0; i < nd; ++i) {
        for (int j = 0; j < ne; ++j) {
            g[i + j] += d[i] * e[j];
        }
    }

    /* Assemble the result: coefficients modulo 10, then convert to integer */
    seq_t result = 0;
    seq_t pow10  = 1;
    int len = nd + ne - 1;          /* number of digit positions produced */

    for (int k = 0; k < len; ++k) {
        int digit = g[k] % 10;
        result += (seq_t)digit * pow10;
        pow10 *= (seq_t)10;
    }
    return result;
}
