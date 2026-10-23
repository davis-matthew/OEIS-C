/*
 * A046616  First numerator and then denominator of the elements to the right
 *          of the central elements of the 1/5-Pascal triangle (by row),
 *          excluding 1's and 5's.
 * https://oeis.org/A046616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046616: numerators of 1/5‑Pascal triangle entries to the right
   of the central elements, excluding values equal to 1 or 5.  The triangle
   is generated row by row with the recurrence
   S[0] = 5;   S[i] = S[i-1] + S[i]  (for i>0), except that the
   interior entry of row 2 is set to 1 because 5+5 = 10.  The values
   obtained are the integers `s` (scaled by a factor of 5).  For each
   entry `s` to the right of the central element(s), the value returned
   is
        num = (s % 5 == 0) ? s / 5 : s,
   which is the numerator in the reduced form of the rational entry.
   The function yields the n‑th such numerator (1‑based).  Complexity
   is O(r²) where r≈2√n (≈200 for n=10⁴). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* The build supplies suitable typedefs for seq_t and idx_t. */
extern seq_t _dummy_seq_t;  /* for the compiler to recognize seq_t type */
extern idx_t _dummy_idx_t;  /* for the compiler to recognize idx_t type */

#define MAX_ROW_LEN 5000  /* sufficient for n up to 10⁴ */

seq_t a(idx_t n)
{
    size_t before = 0;   /* count of terms seen so far */
    size_t r = 0;        /* current row index */
    seq_t cur[MAX_ROW_LEN];
    size_t cur_len = 1;
    cur[0] = (seq_t)5;   /* row 0: single entry 5 (representing 1) */

    while (true)
    {
        /* process entries to the right of the central element(s) */
        size_t start = (r / 2) + 1;
        for (size_t i = start; i < cur_len; ++i)
        {
            seq_t s = cur[i];
            if (s == (seq_t)5)               /* value 1 */
                continue;
            if (s == (seq_t)25)              /* value 5 (unlikely but safe) */
                continue;
            seq_t num = (s % (seq_t)5 == 0) ? s / (seq_t)5 : s;
            ++before;
            if (before == (size_t)n)
                return num;
        }

        /* generate next row */
        size_t next_len = cur_len + 1;
        seq_t next[MAX_ROW_LEN];
        next[0]          = (seq_t)5;
        next[next_len-1] = (seq_t)5;

        for (size_t i = 1; i < next_len - 1; ++i)
        {
            seq_t left  = cur[i-1];
            seq_t right = cur[i];
            seq_t sum   = left + right;
            if (left == (seq_t)5 && right == (seq_t)5)
                next[i] = (seq_t)1;     /* special case for row 2 (5+5=10 → 1) */
            else
                next[i] = sum;
        }

        /* copy next row into current row for next iteration */
        if (next_len > MAX_ROW_LEN)         /* defensive check */
            break;                           /* should never happen for n≤10⁴ */

        for (size_t i = 0; i < next_len; ++i)
            cur[i] = next[i];
        cur_len = next_len;
        ++r;
    }

    return (seq_t)0;   /* unreachable for valid inputs */
}
