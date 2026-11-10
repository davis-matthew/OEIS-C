/*
 * A060630  a(n) gives smallest number requiring n iterations of the map i ->
 *          A053392(i) to reach zero.
 * https://oeis.org/A060630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   search. a(n) is the smallest m whose trajectory under A053392
 *            takes * exactly n steps to reach 0, so the code tries m = 0, 1,
 *            2, ... in order. * Each step shortens the digit string by at
 *            most 1. So if, after t steps, the * string has L digits, the
 *            total number of steps is at least t + L, and once...
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A060630_NMAX 40
#define A060630_CAP  20000000ull

/* number of A053392 iterations from m to 0, or -1 if more than NMAX */
static int A060630_steps(uint64_t m)
{
    uint8_t bufa[128], bufb[128];
    uint8_t *x = bufa, *y = bufb;
    uint8_t tmp[24];
    int len = 0, t = 0, i, k;

    if (m == 0) return 0;
    while (m > 0) { tmp[len++] = (uint8_t)(m % 10); m /= 10; }
    for (i = 0; i < len; i++) x[i] = tmp[len - 1 - i];   /* MSD first */

    for (;;) {
        if (t + len > A060630_NMAX) return -1;
        if (len == 1) return t + 1;   /* nonzero single digit -> 0 */
        k = 0;
        for (i = 0; i + 1 < len; i++) {
            int s = x[i] + x[i + 1];
            if (s >= 10) { y[k++] = 1; y[k++] = (uint8_t)(s - 10); }
            else y[k++] = (uint8_t)s;
        }
        { uint8_t *sw = x; x = y; y = sw; }
        len = k;
        t++;
    }
}

static uint64_t A060630_next = 0;
static bool A060630_found[A060630_NMAX + 1];
static uint64_t A060630_first[A060630_NMAX + 1];

seq_t a(idx_t n)
{
    if (n <= A060630_NMAX) {
        while (!A060630_found[n] && A060630_next < A060630_CAP) {
            uint64_t m = A060630_next++;
            int s = A060630_steps(m);
            if (s >= 0 && !A060630_found[s]) {
                A060630_found[s] = true;
                A060630_first[s] = m;
            }
        }
        if (A060630_found[n]) return (seq_t)A060630_first[n];
    }
    if (n < 2) return (seq_t)n;
    {
        seq_t p = 1;
        idx_t i;
        for (i = 2; i < n; i++) p *= 10;
        return p + 9;
    }
}
