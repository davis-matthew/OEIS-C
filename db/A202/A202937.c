/*
 * A202937  Number of (n+3) X 9 binary arrays with consecutive windows of four
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) (for n > 10, with a(1)…a(10) given below)
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

#ifdef __cplusplus
#include <type_traits>
#endif

/* Helper for multiplying a seq_t by a small unsigned int.
   For built‑in integers we use a 128‑bit intermediate to avoid overflow.
   For arbitrary‑precision types we rely on operator* defined by the class. */
#if defined(__cplusplus)
static inline seq_t mul_small(seq_t x, unsigned int c)
{
    if constexpr (std::is_integral_v<seq_t>) {
        return (seq_t)(((__int128)x) * c);
    } else {
        return x * (seq_t)c;
    }
}
#else
static inline seq_t mul_small(seq_t x, unsigned int c)
{
    return (seq_t)(((__int128)x) * c);
}
#endif

/* Initial values a(1)..a(10).  index 0 is unused so that a(1) is init[1] */
static const seq_t init[11] = {
    0,                     /* unused for n = 0 */
    194481,
    359970,
    695114,
    1360328,
    2631416,
    4958318,
    9044252,
    15949741,
    27226555,
    45087162
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    if (n <= 10)
        return init[(size_t)n];

    /* Buffer holding the last ten values:
       buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[9] = a(k-10) */
    seq_t buf[10];
    buf[0] = init[10];      /* a(10) */
    buf[1] = init[9];
    buf[2] = init[8];
    buf[3] = init[7];
    buf[4] = init[6];
    buf[5] = init[5];
    buf[6] = init[4];
    buf[7] = init[3];
    buf[8] = init[2];
    buf[9] = init[1];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t pos = 0;
        pos += mul_small(buf[0], 10);
        pos += mul_small(buf[2], 120);
        pos += mul_small(buf[4], 252);
        pos += mul_small(buf[6], 120);
        pos += mul_small(buf[8], 10);

        seq_t neg = 0;
        neg += mul_small(buf[1], 45);
        neg += mul_small(buf[3], 210);
        neg += mul_small(buf[5], 210);
        neg += mul_small(buf[7], 45);
        neg += buf[9];                     /* coefficient 1 */

        seq_t new_val = pos - neg;           /* pos >= neg for valid n */

        /* rotate buffer left: buf[k] becomes buf[k-1] */
        for (int k = 9; k > 0; --k)
            buf[k] = buf[k - 1];
        buf[0] = new_val;
    }

    return buf[0];                 /* a(n) */
}
