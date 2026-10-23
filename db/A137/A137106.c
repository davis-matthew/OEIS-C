/*
 * A137106  Numbers k such that k and k^2 use only the digits 2, 4, 7 and 8.
 * https://oeis.org/A137106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   digit_counts, seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A137106 by enumerating all decimal numbers whose
   digits belong to {2,4,7,8} and selecting those for which the
   square also uses only these digits. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_from_str.h"
#include "../utils/digit_counts.h"

/* Returns true if all decimal digits of x belong to {2,4,7,8}. */
static bool all_allowed(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    return cnt[0] == 0 && cnt[1] == 0 && cnt[3] == 0 &&
           cnt[5] == 0 && cnt[6] == 0 && cnt[9] == 0;
}

/* Recursive generator of numbers of a fixed length.
   buf must have at least len+1 bytes. */
static void generate_len_rec(size_t pos, size_t len, char *buf,
                             idx_t target, idx_t *found, seq_t *result)
{
    if (*found >= target)
        return;
    if (pos == len) {
        buf[pos] = '\0';
        seq_t k = seq_from_str(buf);
        seq_t sq = seq_mul(k, k);
        if (all_allowed(sq)) {
            (*found)++;
            if (*found == target)
                *result = k;
        }
        return;
    }
    static const char digits[4] = { '2', '4', '7', '8' };
    for (int di = 0; di < 4; di++) {
        buf[pos] = digits[di];
        generate_len_rec(pos + 1, len, buf, target, found, result);
        if (*found >= target)
            return;
    }
}

/* seq_t a(idx_t n)
   Returns the n-th term of the sequence (a(1) = 2).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1; guard for 0 */

    idx_t cur = 0;
    seq_t result = 0;
    size_t len = 1;

    /* Buffer large enough for the largest numbers we generate. */
    char buf[2048];

    while (cur < n) {
        generate_len_rec(0, len, buf, n, &cur, &result);
        if (cur >= n)
            break;
        ++len;
    }
    return result;
}
