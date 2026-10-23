/*
 * A135901  Number of terms in row n of irregular triangle A135879 for n>=0.
 * https://oeis.org/A135901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + n + 1 - (n+1)*cnt + sum(b_k) where cnt = number of
 *            k ≥ 1 with b_k ≤ n sum(b_k) = Σ b_k over the same k and b_k =
 *            ceil( (2^(k+1) + 6k + 3 + (-1)^k) / 12 )
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Basic term n^2 + n + 1 */
    seq_t nn = (seq_t)n;
    seq_t base = nn * nn + nn + 1;

    seq_t cnt   = 0;
    seq_t sum_b = 0;

    for (uint64_t k = 1; ; ++k) {
        /* Avoid shifting by ≥128 bits (undefined) */
        if (k + 1 >= 128) break;

        /* Compute 2^(k+1) as unsigned 128‑bit integer */
        unsigned __int128 pow2 = (unsigned __int128)1 << (k + 1);

        /* Compute the numerator a = 2^(k+1) + 6k + 3 + (-1)^k */
        unsigned __int128 a_num = pow2 + (unsigned __int128)6 * k + 3;
        if (k & 1)      /* odd k => (-1)^k = -1 */
            a_num -= 1;
        else            /* even k => (-1)^k = +1 */
            a_num += 1;

        /* ceil division by 12 : (a + 11) / 12 */
        unsigned __int128 b128 = (a_num + 11) / 12;
        seq_t b = (seq_t)b128;

        if (b > n) break;   /* further b_k will be larger */

        cnt   += 1;
        sum_b += b;
    }

    /* a(n) = base - (n+1)*cnt + sum_b */
    seq_t result = base - (nn + 1) * cnt + sum_b;
    return result;
}
