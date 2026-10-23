/*
 * A045110  Numbers whose base-4 representation contains no 1's and exactly
 *          one 3.
 * https://oeis.org/A045110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045110: Numbers whose base‑4 representation contains no 1's and exactly one 3.  */
/*   The algorithm enumerates all base‑4 strings of a given length in lexicographic order
    and maps the index n to the appropriate string, converting it to decimal.  */
/*   The implementation uses only integer arithmetic compatible with all supported
    seq_t/idx_t types today.                                                */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If called with n == 0 the behavior is undefined; return 0. */
    if (n == 0) return (seq_t)0;

    /* Find the digit length L that contains the n‑th number. */
    seq_t target = (seq_t)n;
    seq_t cum = (seq_t)0;
    idx_t L = 1;
    seq_t pow2 = 1;        /* 2^(L-2) for the current L (valid for L >= 2) */

    for (;; ++L) {
        seq_t count;
        if (L == 1) {
            count = (seq_t)1;          /* only number 3 */
        } else {
            count = pow2 * ((seq_t)(L + 1));   /* (L+1) * 2^(L-2) */
        }
        if (target <= cum + count) break;
        cum += count;
        if (L >= 2) pow2 <<= 1;   /* multiply by 2 for the next length   */
    }

    /* Off‑set within the block of numbers of length L (0‑based). */
    seq_t offset = target - cum - (seq_t)1;

    /* For length 1 the only number is 3. */
    if (L == 1) return (seq_t)3;

    /* Pre‑compute powers of 2 up to L-1 (for suffix counting). */
    /* 2^k is used many times; buffers are small (L <= 64 for typical n). */
    seq_t pow2_arr[128];
    pow2_arr[0] = (seq_t)1;
    for (idx_t i = 1; i < 128; ++i) pow2_arr[i] = pow2_arr[i - 1] << 1;   /* *2 */

    /* Pre‑compute counts of strings of length k with exactly one 3:
           count_exact[k] = k * 2^(k-1)   (k >= 1),  count_exact[0] = 0.   */
    seq_t count_exact[128];
    count_exact[0] = (seq_t)0;
    for (idx_t k = 1; k < 128; ++k)
        count_exact[k] = (seq_t)k * pow2_arr[k - 1];

    /* Digit array for the base‑4 representation.  Use small fixed size. */
    unsigned char digits[128] = {0};

    /* ------------------------------------------------------------- */
    /*   Decompose the offset into the appropriate digit string.     */
    /* ------------------------------------------------------------- */

    if (offset < (seq_t)(L - 1) * pow2_arr[L - 2]) { /* msd = 2  block   */
        digits[L - 1] = 2;
        bool need3 = true;          /* still have to place the single 3 */
        seq_t rem_offset = offset;

        for (idx_t pos = L - 2; pos != (idx_t)-1; --pos) { /* most to least  */
            /* try digits in ascending order: 0, 2, 3 */
            for (int d_index = 0; d_index < 3; ++d_index) {
                int dig = (d_index == 0) ? 0 : (d_index == 1 ? 2 : 3);
                if (!need3 && dig == 3) continue;   /* 3 already used */

                seq_t count;
                if (need3) {
                    if (dig == 3) count = pow2_arr[pos];
                    else          count = count_exact[pos];   /* rem positions must have one 3 */
                } else { /* need3 already false */
                    count = pow2_arr[pos];   /* all remaining digits are 0 or 2 */
                }

                if (rem_offset < count) {
                    digits[pos] = (unsigned char)dig;
                    if (dig == 3) need3 = false;
                    break; /* move to next position */
                } else {
                    rem_offset -= count;
                }
            }
        }
    } else { /* msd = 3 block   */
        digits[L - 1] = 3;
        seq_t rem_offset = offset - (seq_t)(L - 1) * pow2_arr[L - 2];

        for (idx_t pos = L - 2; pos != (idx_t)-1; --pos) {
            /* only digits 0 or 2 are allowed; 0 comes before 2 */
            seq_t part = pow2_arr[pos];   /* number of suffixes after choosing a digit */
            if (rem_offset < part) {
                digits[pos] = 0;
            } else {
                rem_offset -= part;
                digits[pos] = 2;
            }
        }
    }

    /* Convert the base‑4 digit array to decimal. */
    seq_t result = (seq_t)0;
    seq_t pow4 = (seq_t)1;   /* 4^0 */
    for (idx_t i = 0; i < L; ++i) {
        if (digits[i]) result += (seq_t)digits[i] * pow4;
        pow4 = pow4 << 2; /* multiply by 4 */
    }

    return result;
}
