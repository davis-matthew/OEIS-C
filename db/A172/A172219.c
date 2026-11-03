/*
 * A172219  Number of ways to place 4 nonattacking nightriders on a 4 X n
 *          board.
 * https://oeis.org/A172219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 47 terms through n = 898 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for n>=18: a(n) = (32*n^4-432*n^3+3190*n^2-13323*n+25530)/3.
   For n<18 the value is computed by brute‑force enumeration of all
   4‑combinations of squares on a 4×n board and checking that no two
   nightriders attack each other. */
#include <stdbool.h>
#include <stdint.h>

/* Return true if two nightriders placed at (r1,c1) and (r2,c2)
   attack each other.  For a nightrider the attack vector is a
   knight move repeated any positive integer number of times, so one
   coordinate must be twice the other. */
static bool attacks(int r1, int c1, int r2, int c2)
{
    int dr = r1 > r2 ? r1 - r2 : r2 - r1;
    int dc = c1 > c2 ? c1 - c2 : c2 - c1;
    if (dr == 0 || dc == 0) return false;
    return (dr == 2 * dc && dc > 0) || (dc == 2 * dr && dr > 0);
}

/* Count arrangements of 4 non‑attacking nightriders on a 4×n board. */
static seq_t count4ways(idx_t n)
{
    idx_t cells = 4 * n;
    seq_t total = 0;
    for (idx_t i = 0; i < cells - 3; ++i) {
        int r1 = i / n;
        int c1 = i % n;
        for (idx_t j = i + 1; j < cells - 2; ++j) {
            int r2 = j / n;
            int c2 = j % n;
            if (attacks(r1, c1, r2, c2)) continue;
            for (idx_t k = j + 1; k < cells - 1; ++k) {
                int r3 = k / n;
                int c3 = k % n;
                if (attacks(r1, c1, r3, c3) || attacks(r2, c2, r3, c3)) continue;
                for (idx_t l = k + 1; l < cells; ++l) {
                    int r4 = l / n;
                    int c4 = l % n;
                    if (attacks(r1, c1, r4, c4) ||
                        attacks(r2, c2, r4, c4) ||
                        attacks(r3, c3, r4, c4))
                        continue;
                    ++total;
                }
            }
        }
    }
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle the small values by brute‑force enumeration (only once). */
    static seq_t cache[17];
    static bool ready = false;
    if (!ready) {
        for (idx_t i = 1; i <= 17; ++i)
            cache[i - 1] = count4ways(i);
        ready = true;
    }
    if (n <= 17)
        return cache[n - 1];

    /* Use the polynomial formula for n >= 18. */
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;          /* n^2 */
    seq_t n3   = n2 * sn;          /* n^3 */
    seq_t n4   = n2 * n2;          /* n^4 */

    /* Positive part: 32*n^4 + 3190*n^2 + 25530 */
    seq_t pos = 32ULL * n4;
    pos += 3190ULL * n2;
    pos += 25530ULL;

    /* Negative part: 432*n^3 + 13323*n */
    seq_t neg = 432ULL * n3;
    neg += 13323ULL * sn;

    /* Exact integer result. */
    seq_t num = pos - neg;  /* guaranteed non‑negative for n>=18 */
    return num / 3ULL;
}
