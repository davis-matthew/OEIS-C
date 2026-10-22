/*
 * A003078  Number of letters in n (in Danish).
 * https://oeis.org/A003078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the Danish name of n, built from the
 *            word lengths of the units, teens and tens (halvfjerds = 10,
 *            halvfems = 8), "og" (2), "hundrede" (8), "tusind" (6), "million"
 *            (7)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   count_under_1000
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_under_1000.h"

seq_t a(idx_t n) {
    if (n < 1000) {
        return (seq_t)count_under_1000((int)n);
    }

    if (n < 1000000) {
        int q = (int)(n / 1000);
        int r = (int)(n % 1000);

        int result = count_under_1000(q) + 6;
        if (r > 0) {
            result += 2 + count_under_1000(r);
        }

        return (seq_t)result;
    }

    int q = (int)(n / 1000000);
    int r = (int)(n % 1000000);

    int result = count_under_1000(q) + 7;
    if (r > 0) {
        result += 2;
        if (r < 1000) {
            result += count_under_1000(r);
        } else {
            int q2 = r / 1000;
            int r2 = r % 1000;
            result += count_under_1000(q2) + 6;
            if (r2 > 0) {
                result += 2 + count_under_1000(r2);
            }
        }
    }

    return (seq_t)result;
}
