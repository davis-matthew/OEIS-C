/*
 * A079936  Greedy frac multiples of sqrt(5): a(1)=1, sum(n>0,frac(a(n)*x))=1
 *          at x=sqrt(5).
 * https://oeis.org/A079936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(6n+1)=A001076(4n+1), a(6n+2)=2*a(6n+1),
 *            a(6n+3)=A001076(4n+1)+A001076(4n+2),
 *            a(6n+4)=A001076(4n+3)-A001076(4n+2), a(6n+5)=A001076(4n+3),
 *            a(6n+6)=2*a(6n+5) with A001076(1)=1, A001076(2)=4, and
 *            A001076(k+2)=4*A001076(k+1)+A001076(k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *a001 = NULL;
    static size_t a001_len = 0;

    if (a001_len == 0) {
        a001_len = 2;
        a001 = (seq_t *)malloc(sizeof(seq_t) * a001_len);
        a001[0] = (seq_t)1;
        a001[1] = (seq_t)4;
    }

    /* Compute the block index k and the position within the block r. */
    idx_t k = (n - 1u) / 6u;
    idx_t r = (n - 1u) % 6u;

    /* The largest A001076 index needed for this n is 4*k+3 (1‑based). */
    size_t needed_idx = (size_t)(4u * k + 3u);

    if (needed_idx > a001_len) {
        size_t new_len = needed_idx;
        seq_t *tmp = (seq_t *)realloc(a001, sizeof(seq_t) * new_len);
        for (size_t i = a001_len; i < new_len; ++i) {
            tmp[i] = tmp[i - 1] * (seq_t)4 + tmp[i - 2];
        }
        a001 = tmp;
        a001_len = new_len;
    }

    seq_t result = 0;
    switch (r) {
        case 0: { /* 6k+1 */
            size_t pos = (size_t)(4u * k + 1u);
            result = a001[pos - 1];
            break;
        }
        case 1: { /* 6k+2 */
            size_t pos = (size_t)(4u * k + 1u);
            result = a001[pos - 1];
            result += result;           /* double */
            break;
        }
        case 2: { /* 6k+3 */
            size_t pos = (size_t)(4u * k + 1u);
            result = a001[pos - 1] + a001[pos];
            break;
        }
        case 3: { /* 6k+4 */
            size_t pos = (size_t)(4u * k + 3u);
            result = a001[pos - 1] - a001[pos - 2];
            break;
        }
        case 4: { /* 6k+5 */
            size_t pos = (size_t)(4u * k + 3u);
            result = a001[pos - 1];
            break;
        }
        case 5: { /* 6k+6 */
            size_t pos = (size_t)(4u * k + 3u);
            result = a001[pos - 1];
            result += result;           /* double */
            break;
        }
    }

    return result;
}
