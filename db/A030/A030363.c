/*
 * A030363  Write (n+1)st Fibonacci number in base 3 and juxtapose.
 * https://oeis.org/A030363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n‑th digit of the concatenation of Fibonacci numbers
 *            (starting with F₂) written in base 3. Each Fibonacci number is
 *            written in the usual most‑significant‑digit‑first order
 * Range:     u8 n <= 40, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* external types provided by the build system */
/* seq_t and idx_t are predefined; do not redefine them */

seq_t a(idx_t n)
{
    /* first two Fibonacci numbers (F₁ = 1, F₂ = 1) */
    seq_t prev = 1;          /* F₁ */
    seq_t curr = 1;          /* F₂ */

    /* zero‑based target position in the concatenated string */
    idx_t target = n - 1;

    idx_t consumed = 0;      /* number of digits seen so far */

    /* buffer for base‑3 digits, least‑significant digit first.
       The maximal base‑3 length needed for the range of n of interest is far below 2048. */
    static const size_t MAX_DIGITS = 2048;
    uint8_t buf[MAX_DIGITS];

    while (1) {
        /* convert the current Fibonacci number to base 3 */
        seq_t value = curr;
        size_t len = 0;

        if (value == 0) {          /* for completeness – Fibonacci never zero here */
            buf[0] = 0;
            len = 1;
        } else {
            while (value > 0) {
                buf[len++] = (uint8_t)(value % 3);  /* least‑significant digit */
                value /= 3;
            }
        }

        /* does the target digit lie within this number? */
        if (consumed + (idx_t)len > target) {
            size_t offset = (size_t)(target - consumed);   /* position within the number (0‑based) */
            /* most‑significant digit corresponds to index len‑1‑offset */
            return (seq_t)buf[len - 1 - offset];
        }

        /* move to the next Fibonacci number */
        consumed += (idx_t)len;
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
}
