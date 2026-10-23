/*
 * A049548  a(n+1) = smallest number not containing any digits of a(n),
 *          working in base 4.
 * https://oeis.org/A049548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A049548
   a(n) = smallest number in base 4 that contains no digit of a(n‑1)
   For n > 9:  a(n) = 4·a(n‑2) + (a(n‑2) mod 4)
   Initial values a(0)…a(9) in decimal:
        0, 1, 2, 3, 4, 10, 12, 21, 32, 53
   The implementation uses a circular buffer of length 10 to
   maintain the values required by the recurrence. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        0, 1, 2, 3, 4,          /* a(0) – a(4)   */
        10, 12, 21, 32, 53       /* a(5) – a(9)   */
    };

    /* Return the known values for n < 10 */
    if (n < 10)
        return init[(size_t)n];

    /* Circular buffer: indices 0…9 are reused in cyclic order.
       The recurrence only needs a(i‑2) and a(i‑4), so 10 slots are plenty
       to avoid overwriting a value before it is used. */
    seq_t buf[10];
    for (size_t k = 0; k < 10; ++k)
        buf[k] = init[k];

    for (idx_t i = 10; i <= n; ++i) {
        size_t idx = (size_t)i % 10;          /* position to store a(i)   */
        size_t src = ((size_t)i - 2) % 10;    /* a(i‑2) is stored here   */
        buf[idx] = buf[src] * (seq_t)4 + (buf[src] % (seq_t)4);
    }

    return buf[(size_t)n % 10];
}
