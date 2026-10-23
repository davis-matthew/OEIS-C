/*
 * A152491  Numbers n such that 1/c = 1/n + 1/S(n). c, n positive integers
 *          (A000027(n)), S(n) sum of digits of n (A007953(n)).
 * https://oeis.org/A152491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series of numbers n such that n * sum_of_digits(n) is divisible
   by (n + sum_of_digits(n)).  This is an equivalent condition to
   1/c = 1/n + 1/S(n) for some positive integer c. */

#include <stdint.h>
#include <stddef.h>

/* Single function required by the harness. */
seq_t a(idx_t n)
{
    /* Ensure the sequence is generated once.  The sequence has only
       the six terms 2,4,6,8,18,72.  We generate terms by brute‑force
       test up to an upper bound where all known terms lie. */
    static seq_t seq[10];        /* capacity > 6 */
    static size_t seq_len = 0;
    static int initialized = 0;

    if (!initialized) {
        const idx_t LIMIT = 255;              /* covers all known terms */
        for (idx_t i = 1; i <= LIMIT; ++i) {
            /* Compute sum of decimal digits of i. */
            idx_t temp = i;
            idx_t s = 0;
            while (temp) {
                s += temp % 10;
                temp /= 10;
            }
            /* Check divisibility condition using 128‑bit arithmetic
               to avoid overflow. */
            unsigned __int128 num = (unsigned __int128)i * s;
            unsigned __int128 den = (unsigned __int128)i + s;
            if (num % den == 0) {
                if (seq_len < sizeof(seq)/sizeof(seq[0])) {
                    seq[seq_len++] = (seq_t)i;
                }
            }
        }
        initialized = 1;
    }

    if (n == 0 || n > seq_len)
        return 0;  /* undefined for out of bounds – return 0 */
    return seq[n - 1];
}
