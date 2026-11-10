/*
 * A208852  Smallest m such that A090895(m) = n.
 * https://oeis.org/A208852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A090895(m) = n, where A090895(1) = 1,
 *            A090895(k) = A090895(k-1)/2 if that is even, else A090895(k-1) +
 *            k. Computed by a search: simulate the recurrence and record
 *            first occurrences
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 1097, u64 n <= 1097,
 *            u128 n <= 1097, big any n
 * Validated: standard, pass, 76 terms through n = 1081 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A208852_TABLE 16384

static idx_t A208852_first[A208852_TABLE];
static idx_t A208852_m = 0;
static seq_t A208852_v = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (n < A208852_TABLE) {
        if (A208852_m == 0) {
            A208852_m = 1;
            A208852_v = (seq_t)1;
            A208852_first[1] = 1;
        }
        while (A208852_first[n] == 0) {
            A208852_m++;
            if ((A208852_v & (seq_t)1) == 0) {
                A208852_v = A208852_v >> 1;
            } else {
                A208852_v = A208852_v + (seq_t)A208852_m;
            }
            if (A208852_v < (seq_t)A208852_TABLE) {
                idx_t k = (idx_t)(unsigned long long)A208852_v;
                if (A208852_first[k] == 0) A208852_first[k] = A208852_m;
            }
        }
        return (seq_t)A208852_first[n];
    }

    /* Large target: simulate from scratch */
    {
        idx_t m = 1;
        seq_t v = (seq_t)1;
        while (v != (seq_t)n) {
            m++;
            if ((v & (seq_t)1) == 0) v = v >> 1;
            else v = v + (seq_t)m;
        }
        return (seq_t)m;
    }
}
