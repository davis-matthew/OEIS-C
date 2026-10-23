/*
 * A036110  A summarize Fibonacci sequence starting with a(0)=2 and a(1)=2:
 *          summarize the previous two terms!.
 * https://oeis.org/A036110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=2;
   For n ≥ 2, a(n) is the decimal concatenation of, for each digit d from
   9 down to 0, the count of d in a(n‑1) followed by a(n‑2), but only
   if the count is positive. */
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

/* Return the nth term of A036110. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)2;

    size_t cap = (size_t)n + 1;
    seq_t *seq = seq_alloc_new_array(cap);
    if (!seq)
        return (seq_t)0;           /* out of memory */
    seq[0] = seq[1] = (seq_t)2;

    for (idx_t i = 2; i <= n; ++i) {
        /* Count decimal digits in the two previous terms. */
        int freq[10] = {0};
        seq_t v = seq[i - 1];
        if (v == 0) {
            freq[0] += 1;
        } else {
            seq_t t = v;
            while (t != 0) {
                int d = (int)(t % 10);
                freq[d] += 1;
                t /= 10;
            }
        }
        v = seq[i - 2];
        if (v == 0) {
            freq[0] += 1;
        } else {
            seq_t t = v;
            while (t != 0) {
                int d = (int)(t % 10);
                freq[d] += 1;
                t /= 10;
            }
        }

        /* Build the next term by concatenating (count)(digit) pairs. */
        seq_t new_val = 0;
        for (int d = 9; d >= 0; --d) {
            int cnt = freq[d];
            if (cnt > 0) {
                int digits[32];
                int dlen = 0;
                int tmp = cnt;
                if (tmp == 0)
                    digits[dlen++] = 0;
                while (tmp > 0) {
                    digits[dlen++] = tmp % 10;
                    tmp /= 10;
                }
                for (int k = dlen - 1; k >= 0; --k)
                    new_val = new_val * 10 + (seq_t)digits[k];
                new_val = new_val * 10 + (seq_t)d;
            }
        }
        seq[i] = new_val;
    }

    seq_t result = seq[n];
    free_seq_array(seq);
    return result;
}
