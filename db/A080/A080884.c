/*
 * A080884  Sum of (0,1) versions of Thue-Morse word (A010060) and Fibonacci
 *          word (A003849).
 * https://oeis.org/A080884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Thue‑Morse(n) + FibonacciWord(n)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   fibonacci_word_value, thue_morse_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fibonacci_word_value.h"
#include "../utils/thue_morse_value.h"

seq_t a(idx_t n)
{
    seq_t t = thue_morse_value(n);
    seq_t f = fibonacci_word_value(n);
    return t + f;
}
