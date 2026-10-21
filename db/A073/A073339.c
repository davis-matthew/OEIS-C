/*
 * A073339  Define b(k) by the recursion b(1)=n, b(k+1)=b(k)-trunc(k/b(k)),
 *          where trunc(x) is floor(x) for x>=0, ceiling(x) for x<0. Sequence
 *          gives the value a(n) such that b(a(n))=0; if k>a(n) then b(k) is
 *          undefined.
 * https://oeis.org/A073339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 89, u32 n <= 1800, u64 n <= 1800,
 *            u128 n <= 1800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A073339.
 *
 * For a given n (n ≥ 1) we define
 *
 *   b(1) = n,
 *   b(k+1) = b(k) - trunc(k / b(k)),
 *
 * where trunc(x) = floor(x) if x ≥ 0, and trunc(x) = ceil(x) if x < 0.
 * The function returns the first index a such that b(a) == 0.
 *
 * The computation uses a signed 64‑bit intermediate type int64_t to
 * handle both positive and negative b(k).  The returned value is cast
 * to seq_t.  This satisfies the recurrence definition and works for
 * all values that fit in the scalar ranges used in the test builds.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* n=0 is outside the OEIS range. */
        return (seq_t)0;

    int64_t b = (int64_t)n;     /* current value of b(k). */
    for (idx_t k = 1; ; ++k) {  /* k is the current index, starting at 1. */
        int64_t trunc;

        if (b > 0) {
            trunc = (int64_t)k / b;          /* floor(k / b). */
        } else {                            /* b < 0 : use ceil(k / b). */
            trunc = -((int64_t)k / (int64_t)(-b));
        }

        int64_t next = b - trunc;            /* b(k+1). */
        if (next == 0)                       /* b(a) == 0 → return a. */
            return (seq_t)(k + 1);

        b = next;
    }
}
