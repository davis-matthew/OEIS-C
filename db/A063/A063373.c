/*
 * A063373  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          100 ).
 * https://oeis.org/A063373
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063373: dimension of the space of weight n cuspidal newforms for Γ₁(100)
   Recurrence (valid for n > 13):  a(n) = a(n-4) + a(n-6) - a(n-10)
   Initial values: a(2) = -1, a(3) = 141, a(4) = 302, a(5) = 463, a(6) = 625,
   a(7) = 786, a(8) = 948, a(9) = 1107, a(10) = 1271, a(11) = 1430,
   a(12) = 1594, a(13) = 1752. */

#include <stdlib.h> /* for idx_t type, if needed */

/* Pre‑initialised values for 2 ≤ n ≤ 13.  index 0,1 are unused. */
static const seq_t init_vals[14] = {
    (seq_t)0,   /* a(0) – unused */
    (seq_t)0,   /* a(1) – unused */
    (seq_t)-1,  /* a(2) */
    (seq_t)141,
    (seq_t)302,
    (seq_t)463,
    (seq_t)625,
    (seq_t)786,
    (seq_t)948,
    (seq_t)1107,
    (seq_t)1271,
    (seq_t)1430,
    (seq_t)1594,
    (seq_t)1752
};

seq_t a(idx_t n)
{
    /* undefined below the offset */
    if (n < 2)
        return (seq_t)0;

    /* handle the initial segment directly */
    if (n <= 13)
        return init_vals[(size_t)n];

    /* Use a small circular buffer to keep the last 15 values.
       The recurrence only needs a(n-4), a(n-6) and a(n-10). */
    enum { BUF_SZ = 15 };
    seq_t buf[BUF_SZ];
    for (int i = 0; i < BUF_SZ; ++i)
        buf[i] = (seq_t)0;

    /* Fill the buffer with the known initial values */
    for (idx_t i = 2; i <= 13; ++i)
        buf[i % BUF_SZ] = init_vals[(size_t)i];

    /* Compute successive terms until n */
    for (idx_t cur = 14; cur <= n; ++cur) {
        seq_t val = buf[(cur - 4) % BUF_SZ] + buf[(cur - 6) % BUF_SZ] - buf[(cur - 10) % BUF_SZ];
        buf[cur % BUF_SZ] = val;
        if (cur == n)
            return val;
    }

    /* This return is never reached, but keeps the compiler happy. */
    return (seq_t)0;
}
