/*
 * A095076  Parity of 1-fibits in Zeckendorf expansion A014417(n).
 * https://oeis.org/A095076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = parity of the number of Fibonacci summands in the *
 *            Zeckendorf representation of n (i.e. a(n) = 0 if that number is
 *            even, * 1 if it is odd). The function computes the Zeckendorf
 *            greedy * expansion using only O(log n) time and O(1) memory
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder to be represented. */
    idx_t rem = n;
    /* parity of the count of Fibonacci summands (0 = even, 1 = odd). */
    unsigned int parity = 0;

    while (rem > 0) {
        /* Find the largest Fibonacci number <= rem. */
        idx_t a_fib = 1, b_fib = 2;
        while (b_fib <= rem) {
            idx_t next = a_fib + b_fib;
            /* Detect unsigned overflow and stop if it occurs. */
            if (next <= a_fib)        /* overflow happened */
                break;
            a_fib = b_fib;
            b_fib = next;
        }

        /* Subtract this Fibonacci number and flip parity. */
        rem -= a_fib;
        parity ^= 1;
    }

    return (seq_t)parity;   /* 0 or 1 as a seq_t */
}
