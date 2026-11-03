/*
 * A033862  Sorted number reached from starter n in Sort-then-add sequence, or
 *          -1 if never reaches sorted number.
 * https://oeis.org/A033862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 314, u32 n <= 315, u64 n <= 315,
 *            u128 n <= 315, big any n
 * Validated: standard, pass, 87 terms through n = 298 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sorted number reached from starter n in Sort‑then‑add sequence
 * (OEIS A033862).  For a starting integer n (n ≥ 1) repeatedly
 * replace n by n + a10(n) where a10(n) is the integer obtained
 * by ordering the decimal digits of n in non‑decreasing order
 * (leading zeros are discarded).  The process stops when the new
 * n is already sorted, returning that sorted number.  If the
 * sequence never reaches a sorted number, the function returns
 * (seq_t)-1.
 *
 * Example: n = 64
 *   64 → 64 + 46 = 110
 *   110 → 110 + 11 = 121
 *   121 → 121 + 112 = 233  (sorted) → return 233.
 */

#include <stdint.h>   /* for uint8_t */

/* Compute the ascending digit number of x (leading zeros are dropped). */
static seq_t asc_digits(seq_t x)
{
    uint8_t cnt[10] = {0};
    seq_t tmp = x;

    /* Count digits. */
    while (tmp > 0) {
        cnt[(int)(tmp % 10)]++;
        tmp /= 10;
    }

    /* Build the ascending number. */
    seq_t asc = 0;
    for (int d = 0; d < 10; ++d) {
        uint8_t t = cnt[d];
        while (t--) {
            asc = asc * 10 + (seq_t)d;
        }
    }
    return asc;
}

/* Next number in the sequence: n + asc_digits(n). */
static inline seq_t next_num(seq_t n)
{
    return n + asc_digits(n);
}

/* Main sequence function.  Uses Floyd’s tortoise‑and‑hare cycle detection. */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;

    /* If the original number is already sorted, return it. */
    if (asc_digits(cur) == cur) {
        return cur;
    }

    /* Tortoise moves one step, hare moves two steps. */
    seq_t tortoise = next_num(cur);
    if (asc_digits(tortoise) == tortoise) {
        return tortoise;
    }

    seq_t hare = next_num(tortoise);
    /* hare already advanced two steps (next_num(next_num(cur))) */

    /* Floyd’s algorithm. */
    while (tortoise != hare) {
        tortoise = next_num(tortoise);
        if (asc_digits(tortoise) == tortoise) {
            return tortoise;
        }

        hare = next_num(hare);
        if (asc_digits(hare) == hare) {
            return hare;
        }

        hare = next_num(hare);
        if (asc_digits(hare) == hare) {
            return hare;
        }
    }

    /* Cycle detected without finding a sorted number. */
    return (seq_t)-1;
}
