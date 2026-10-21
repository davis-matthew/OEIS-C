/*
 * A098756  Smallest available integer which fits into the repeating pattern
 *          9876543210.
 * https://oeis.org/A098756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A098756:
   a(1..10) = 9,8,7,6,5,4,3,2,1,0.
   For n>10:
     Let d = last digit of a(n‑1).
     Find the greatest i < n with first digit of a(i) = (d‑1 mod 10).
     Let b = a(i).
     Let e be the digit that follows the last digit of b in the cycle
        9,8,7,6,5,4,3,2,1,0,9,...
     If e == 1, augment b with the two digits 1,0; otherwise augment with e.
     a(n) = b * 10 + e  (or b * 100 + 10 when e==1).
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t last_idx[10];
    const int init_vals[10] = {9,8,7,6,5,4,3,2,1,0};
    static const int next_of[10] = {9,0,1,2,3,4,5,6,7,8};

    if (n == 0) return (seq_t)0;

    while (len < (size_t)n) {
        if (cap == 0) {
            cap = 16;
            seq = (seq_t*)malloc(cap * sizeof(seq_t));
        } else if (len == cap) {
            cap <<= 1;
            seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
        }
        idx_t idx = (idx_t)len + 1;            /* 1‑based index of the new element */

        seq_t val;
        if (idx <= 10) {
            val = (seq_t)init_vals[(size_t)idx - 1];
        } else {
            seq_t prev = seq[(size_t)idx - 2];     /* a(n‑1) */
            int last_digit = (int)(prev % 10);
            int target_first = (last_digit + 9) % 10;  /* (d‑1) mod 10 */
            idx_t i = last_idx[target_first];
            if (i == 0) i = 1;       /* safety, should not happen for n>10 */
            seq_t base = seq[(size_t)i - 1];
            int base_last = (int)(base % 10);
            int next_digit = next_of[base_last];
            if (next_digit == 1) {
                val = base * (seq_t)100 + (seq_t)10;
            } else {
                val = base * (seq_t)10 + (seq_t)next_digit;
            }
        }

        seq[(size_t)len] = val;

        /* Update most recent index for the first digit of val */
        int fd = (int)val;
        while (fd >= 10) fd /= 10;
        last_idx[fd] = idx;

        ++len;
    }
    return seq[(size_t)n - 1];
}
