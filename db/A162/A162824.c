/*
 * A162824  Positive numbers k such that 48*k/(48-k) are integers.
 * https://oeis.org/A162824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth positive integer k such that 48*k/(48-k) is an
 *            integer. The condition is equivalent to (48-k) | 48*48, i.e.
 *            (48-k) divides 2304. Therefore all such k are given by k = 48 ±
 *            d, where d runs over all positive divisors of 2304, together
 *            with the special case k = 48. The sequence is fi...
 * Range:     u8 n <= 14, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* static cache of the sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (seq == NULL) {
        /* generate all positive divisors of 2304 = 2^8 * 3^2 */
        uint64_t divisors[27];
        int divcnt = 0;
        int pow3[3] = {1, 3, 9};

        for (int a = 0; a <= 8; ++a) {
            uint64_t pow2 = 1ULL << a;          /* 2^a */
            for (int b = 0; b <= 2; ++b) {
                divisors[divcnt++] = pow2 * pow3[b];
            }
        }

        /* there are exactly 27 divisors, therefore 13 < 48 plus
           27 + 1 (k=48) = 41 sequence elements.            */
        seq_len = 41;
        seq = seq_alloc(seq_len);

        size_t idx = 0;
        for (int i = 0; i < divcnt; ++i) {
            uint64_t d = divisors[i];

            if (48 > d) {                          /* k = 48 - d  > 0 */
                seq[idx++] = (seq_t)(48ULL - d);
            }
            seq[idx++] = (seq_t)(48ULL + d);        /* k = 48 + d  */
        }

        /* add the special case k = 48 */
        seq[idx++] = (seq_t)48ULL;

        /* sort the sequence ascending */
        qsort(seq, seq_len, sizeof(seq_t), seq_cmp);
    }

    /* return the nth element (1‑based), or 0 if n exceeds length */
    if (n <= seq_len) {
        return seq[n - 1];
    } else {
        return (seq_t)0;
    }
}
