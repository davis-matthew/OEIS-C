/*
 * A014677  First differences of A001468.
 * https://oeis.org/A014677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A014677: first differences of the Fibonacci word over {2,1} (Wythoff sequence
 *  implementation).  The sequence is defined by the recurrence:
 *    a(0) = 1,
 *    a(n) = -1 if n+1 is a Wythoff B‑number,
 *    a(n) =  1 if n   is a Wythoff B‑number,
 *    a(n) =  0 otherwise.
 *  The Wythoff B sequence is B(k) = ⌊k·φ²⌋ = ⌊k·φ⌋ + k.
 *  This code precomputes the B sequence up to a fixed bound and uses it for all
 *  subsequent queries.  It uses only integer arithmetic. */

#include <stddef.h>
#include <stdbool.h>

#define MAX_VAL 20002          /* must be ≥ maximum n+1 that will be queried */

static bool isB[MAX_VAL];      /* isB[x] == true  ⇔  x is a B‑number   */
static bool initiated = false;

static void init_B(void)
{
    if (initiated) return;

    /*  A rational approximation of φ; Φ ≈ 46368 / 28657 (≈1.61803398875) */
    const idx_t NUM = 46368;
    const idx_t DEN = 28657;

    for (idx_t k = 1; ; ++k)
    {
        /* floor(k·φ) = floor(k·NUM/DEN) */
        seq_t a = (seq_t)k * NUM / DEN;
        seq_t b = a + k;                 /* B(k) = A(k) + k */

        if (b >= MAX_VAL) break;         /* array indices go up to MAX_VAL-1 */
        isB[(size_t)b] = true;
    }

    initiated = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* offset 0 */

    init_B();                              /* ensure B sequence is ready */

    idx_t m = n + 1;                       /* works with formulas using n+1 */

    /* m is a B‑number → a(n) = -1 */
    if (m < MAX_VAL && isB[(size_t)m]) return (seq_t)-1;

    /* m-1 is a B‑number → a(n) = +1 */
    if (m > 1 && isB[(size_t)(m - 1)]) return (seq_t)1;

    /* otherwise a(n) = 0 */
    return (seq_t)0;
}
