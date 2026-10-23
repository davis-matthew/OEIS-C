/*
 * A047930  Smallest positive Fibonacci number divisible by n.
 * https://oeis.org/A047930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = smallest positive Fibonacci number divisible by n.
 *            Compute by naive search: iterate Fibonacci numbers modulo n
 *            until the modulus becomes zero. The resulting Fibonacci value is
 *            the desired term. Complexity O(k) where k is the rank of
 *            apparition of n (≤10000 for the intended test range)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 29, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The Fibonacci numbers are generated in `f0` and `f1` to keep
       both the full value (`seq_t`) and the next value simultaneously.
       The modulo test uses `f1 % n`, with n cast to `seq_t` for
       consistency with the arithmetic type. */
    seq_t f0 = 0;
    seq_t f1 = 1;
    /* For n == 1 the loop exits immediately (1 % 1 == 0). */
    while ((f1 % (seq_t)n) != 0) {
        seq_t fnext = f0 + f1;
        f0 = f1;
        f1 = fnext;
    }
    return f1;
}
