/*
 * A078271  a(n) is the number from which if even-positioned digits are
 *          deleted then one gets the concatenation of 1 to n, while if
 *          odd-positioned digits are deleted then one gets reverse
 *          concatenation of 1 to n. Position of least significant digit is
 *          considered to be 1.
 * https://oeis.org/A078271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is built by interleaving the forward concatenation of 1..n
 *            with the reverse concatenation (n..1). The k‑th digit of the
 *            result is r_k s_k where r_k is the k‑th digit of the string
 *            n(n‑1)…1 and s_k is the k‑th digit of the string 1…n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digits_of, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "../utils/digits_of.h"
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined below offset */
    /* compute total number of digits in the forward concatenation 1..n */
    idx_t total_len = 0;
    for (idx_t i = 1; i <= n; ++i)
        total_len += digits_of(i);

    /* allocate buffers for the forward and reverse concatenations */
    char *S = (char *)malloc((size_t)total_len + 1);
    char *R = (char *)malloc((size_t)total_len + 1);
    if (!S || !R) {
        free(S); free(R);
        return 0;
    }

    /* build forward concatenation 1..n into S */
    size_t pos = 0;
    char tmp[32];
    for (idx_t i = 1; i <= n; ++i) {
        int len = snprintf(tmp, sizeof(tmp), "%" PRIu64, (uint64_t)i);
        memcpy(S + pos, tmp, (size_t)len);
        pos += (size_t)len;
    }
    S[total_len] = '\0';

    /* build reverse concatenation n..1 into R */
    pos = 0;
    for (idx_t i = n; i >= 1; --i) {
        int len = snprintf(tmp, sizeof(tmp), "%" PRIu64, (uint64_t)i);
        memcpy(R + pos, tmp, (size_t)len);
        pos += (size_t)len;
        if (i == 1) break;   /* prevent underflow for idx_t */
    }
    R[total_len] = '\0';

    /* interleave: r1 s1 r2 s2 ... */
    size_t dest_len = (size_t)total_len * 2;
    char *dest = (char *)malloc(dest_len + 1);
    if (!dest) {
        free(S); free(R);
        return 0;
    }

    for (size_t k = 0; k < (size_t)total_len; ++k) {
        dest[2 * k]     = R[k];
        dest[2 * k + 1] = S[k];
    }
    dest[dest_len] = '\0';

    /* convert to seq_t */
    seq_t result = seq_from_str(dest);

    /* free temporary buffers */
    free(S);
    free(R);
    free(dest);

    return result;
}
