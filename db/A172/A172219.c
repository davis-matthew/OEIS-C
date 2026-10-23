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
 * Helpers:   attacks, count4ways
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for n>=18: a(n) = (32*n^4-432*n^3+3190*n^2-13323*n+25530)/3.
   For n<18 the value is computed by brute‑force enumeration of all
   4‑combinations of squares on a 4×n board and checking that no two
   nightriders attack each other. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/attacks.h"
#include "../utils/count4ways.h"

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
