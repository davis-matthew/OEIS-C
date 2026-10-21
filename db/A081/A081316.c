/*
 * A081316  Lexicographically earliest sequence of positive integers such that
 *          n occurs a(n)-1 times, starting at n=1.
 * https://oeis.org/A081316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For n = 1..4, a(n) is fixed: a(1) = 2, a(2) = 3, a(3) = 2, a(4)
 *            = 1. For n ≥ 5, the sequence is built from consecutive blocks.
 *            Let f(i) = 4 + floor((i-5)/4); // number of times integer i must
 *            appear The sequence consists of integer 5 repeated f(5)=4 times,
 *            6 repeated f(6)=4 times, 7 repeated f(7)...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* 1‑based index, a(1)=2 … */
    if (n == 0) return 0;            /* undefined, but safeguard */

    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)1;

    /* For n >= 5 use block iteration. */
    idx_t pos = n - 5;               /* zero‑based offset within the blocks of values >=5 */
    idx_t val = 5;                   /* current integer whose block we are examining */

    while (true) {
        /* f(val) = 4 + floor((val-5)/4) */
        idx_t block_len = 4 + ( (val - 5) / 4 );
        if (pos < block_len) break;  /* the nth position lies in this block */
        pos -= block_len;
        val++;
    }
    return (seq_t) val;
}
