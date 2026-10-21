/*
 * A061819  Multiples of 3 containing only digits 0,1,2,3.
 * https://oeis.org/A061819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 342, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A061819:
   a(n) = the n‑th non‑negative multiple of 3 whose decimal digits are
   all 0, 1, 2 or 3. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Recursive helper:
   rec(target, cnt_ptr, idx, len, val, out) generates all numbers of
   length 'len' with leading digit non‑zero. When 'cnt' reaches
   'target' it stores the found value into '*out' and returns true. */
static bool rec(idx_t target, idx_t *cnt, size_t idx, size_t len,
                seq_t val, seq_t *out)
{
    if (idx == len) {
        if (val % 3 == 0) {
            (*cnt)++;
            if (*cnt == target) {
                *out = val;
                return true;
            }
        }
        return false;
    }
    int start = (idx == 0) ? 1 : 0;   /* leading digit cannot be 0 unless len==1 (handled elsewhere) */
    for (int d = start; d <= 3; ++d) {
        seq_t next = val * 10 + (seq_t)d;
        if (rec(target, cnt, idx + 1, len, next, out))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 1)                /* a(1) = 0 */
        return 0;

    idx_t count = 1;           /* zero already counted */
    seq_t result = 0;

    for (size_t len = 1; ; ++len) {
        if (rec(n, &count, 0, len, 0, &result))
            return result;
    }
}
